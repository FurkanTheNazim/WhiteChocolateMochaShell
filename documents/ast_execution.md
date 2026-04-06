# AST Execution — Executor Nasıl Yürütecek?

## "Sola Yatırma" vs "Sağa Yatırma" Nedir?

Bu sadece **ağacın hangi yöne büyüdüğü** ile ilgili. İkisi de doğru, fark executor'ın nasıl recurse yaptığında.

### Bizim Ağacımız: SAĞ YATIK (Right-leaning)
`cmd1 | cmd2 | cmd3` →
```
     PIPE
    /    \
  cmd1   PIPE
        /    \
      cmd2   cmd3
```
"Sağ taraf büyüyor" — her pipe'ın sağ çocuğu bir sonraki pipe.

### Sol Yatık alternatif (Left-leaning)
```
       PIPE
      /    \
    PIPE   cmd3
   /    \
 cmd1   cmd2
```
"Sol taraf büyüyor" — her pipe'ın sol çocuğu önceki pipe.

### Hangisi doğru?
**İkisi de doğru.** Minishell için sağ yatık daha basit çünkü:
- Sol çocuk her zaman **basit bir komut** (AST_CMD)
- Sağ çocuk ya **basit komut** (son) ya da **başka bir PIPE**
- Executor kodu daha temiz olur

---

## Executor Nasıl Çalışacak? (Adım Adım)

### Temel Mantık (Pseudocode)

```
execute_ast(node):

    Eğer node == AST_CMD:
        → Redirectionları aç (dup2)
        → Builtin mi? → doğrudan çalıştır
        → Değilse? → fork + execve

    Eğer node == AST_PIPE:
        → pipe() çağır → fd[0](oku), fd[1](yaz)
        → fork() sol çocuk:
              stdout'u fd[1]'e bağla (dup2)
              execute_ast(node->left)   ← sol her zaman AST_CMD
        → fork() sağ çocuk:
              stdin'i fd[0]'a bağla (dup2)
              execute_ast(node->right)  ← AST_CMD veya başka PIPE (recursive!)
        → Baba: pipe fd'leri kapat, wait()
```

### Gerçek Örnek: `ls | grep test | wc -l`

Ağaç:
```
         PIPE_A
        /      \
    [ls]      PIPE_B
             /      \
       [grep test] [wc -l]
```

#### Adım 1: `execute_ast(PIPE_A)` çağrılır

```
PIPE_A işleniyor:
  ┌─ pipe() → pipe_a_fd[0]=3, pipe_a_fd[1]=4
  │
  ├─ fork() → Çocuk 1 (PID: 1001)
  │    dup2(pipe_a_fd[1], STDOUT)  ← stdout artık pipe'a yazıyor
  │    close(pipe_a_fd[0], pipe_a_fd[1])
  │    execute_ast(left → AST_CMD "ls")
  │    → execve("/bin/ls")
  │    → ls çıktısı pipe_a'ya yazılır
  │
  ├─ fork() → Çocuk 2 (PID: 1002)
  │    dup2(pipe_a_fd[0], STDIN)   ← stdin artık pipe'tan okuyor
  │    close(pipe_a_fd[0], pipe_a_fd[1])
  │    execute_ast(right → PIPE_B)  ← RECURSIVE ÇAĞRI!
  │    │
  │    └──── Adım 2'ye git
  │
  └─ Baba: close(pipe_a_fd), waitpid()
```

#### Adım 2: `execute_ast(PIPE_B)` çağrılır (Çocuk 2 içinde)

```
PIPE_B işleniyor (stdin zaten pipe_a'dan okuyor):
  ┌─ pipe() → pipe_b_fd[0]=5, pipe_b_fd[1]=6
  │
  ├─ fork() → Çocuk 3 (PID: 1003)
  │    dup2(pipe_b_fd[1], STDOUT)  ← stdout pipe_b'ye yazıyor
  │    close(pipe_b_fd[0], pipe_b_fd[1])
  │    execute_ast(left → AST_CMD "grep test")
  │    → execve("/bin/grep", "grep", "test")
  │    → stdin'den okuyor (pipe_a'dan gelen ls çıktısı!)
  │    → eşleşen satırları pipe_b'ye yazıyor
  │
  ├─ fork() → Çocuk 4 (PID: 1004)
  │    dup2(pipe_b_fd[0], STDIN)   ← stdin pipe_b'den okuyor
  │    close(pipe_b_fd[0], pipe_b_fd[1])
  │    execute_ast(right → AST_CMD "wc -l")
  │    → execve("/usr/bin/wc", "wc", "-l")
  │    → stdin'den okuyor (grep çıktısı!)
  │    → sonucu terminale yazıyor (stdout hala terminal)
  │
  └─ Baba: close(pipe_b_fd), waitpid()
```

#### Sonuç: Veri Akışı

```
ls ──stdout──→ [pipe_a] ──stdin──→ grep test ──stdout──→ [pipe_b] ──stdin──→ wc -l ──stdout──→ TERMİNAL
```

---

## Tek Komut + Redirection

`cat < input.txt > output.txt`

Ağaç:
```
   AST_CMD
     │
     ▼
  t_command
  args: ["cat"]
  redirs: [REDIR_IN "input.txt"] → [REDIR_OUT "output.txt"]
```

Executor:
```
execute_ast(AST_CMD):
  1. redirs listesini dolaş:
     ├─ REDIR_IN "input.txt"
     │    fd = open("input.txt", O_RDONLY)
     │    dup2(fd, STDIN_FILENO)
     │    close(fd)
     │
     └─ REDIR_OUT "output.txt"
          fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC)
          dup2(fd, STDOUT_FILENO)
          close(fd)

  2. execve("/bin/cat")
     → stdin: input.txt'ten okuyor
     → stdout: output.txt'e yazıyor
```

---

## Özet: Ne Recursive, Ne Değil?

| Ne? | Recursive mi? | Nerede? |
|-----|--------------|---------|
| AST **oluşturma** | ✅ Evet | `list_to_ast()` → parser'dan sonra, bir kez çalışır |
| AST **yürütme** | ✅ Evet | `execute_ast()` → her PIPE node'da sağ çocuğa recurse eder |
| Redir işleme | ❌ Hayır | Her komut için sıralı `open() + dup2()` döngüsü |
| Builtin çalıştırma | ❌ Hayır | Doğrudan fonksiyon çağrısı |

> [!TIP]
> **Sağ yatık ağacın güzelliği**: Sol çocuk **her zaman** basit bir komut.
> Executor sol tarafı fork+exec eder, sağ tarafa recursive gider.
> Bu yüzden "sola yatırmaya" gerek yok — sağ yatık, shell pipe'ları için en doğal yapı.

---

## Yukarıdan Aşağıya (Top-Down) Çalışma Mantığı Özeti

Executor'a her zaman ağacın kökü (en tepesi) verilir ve bir "şelale" mantığıyla yukarıdan aşağıya (top-down) çalışır.

Örnek Ağaç: `ls | grep a | wc -l`
```
           [ROOT]
         PIPE (1)
        /        \
    [ls]        PIPE (2)
               /        \
         [grep a]      [wc -l]
```

**Nasıl Çalışır?**
1. **Executor en tepeye (Root PIPE 1'e) gelir:**
   - Bakar ki bu bir PIPE. Hemen asıl boruyu (`pipe()`) oluşturur.
   - **Sol dala (ls)** bakar: "Sen düz bir komutsun, senin stdout'unu borunun girişine bağlayıp çalıştırıyorum" der ve onu aradan çıkarır (`fork` + `execve`).
   - **Sağ dala (PIPE 2)** bakar: "Hmm sen yine bir PIPE imişsin, o zaman ben buraya doğru **aşağıya inmeye devam edeyim (recursive)** ve bu sefer stdin'i ilk borunun çıkışından alayım" der.

2. **Executor bir kademe aşağı iner (PIPE 2'ye):**
   - Burada yine yeni bir boru açar.
   - **Sol dala (grep a)**: Boruya bağlayıp çalıştırır.
   - **Sağ dala (wc -l)**: "Aa sen son dalsın, düz komutsun. Artık aşağı inecek bir şey kalmadı, seni de çalıştırıp bitiriyorum" der.

Tıpkı bir şelale gibi **kökten başlar**, her adımda sol dalı (komutu) çalıştırıp hattan ayırır, sağ dala (kalan komutlara) doğru inerek en alt yaprağa ulaşana kadar devam eder.
