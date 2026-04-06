# AST Sistemi Nasıl Çalışıyor?

## İki Farklı Katman

AST ve redirectionlar **farklı katmanlarda** çalışır. Birbirine karışmasın:

| Katman | Ne yapar? | Nerede? |
|--------|-----------|---------|
| **AST** | Pipe'larla bağlı komutlar arası **akışı** organize eder | `t_ast_node` (tree) |
| **Redir** | Tek bir komutun **stdin/stdout'unu** dosyalara yönlendirir | `t_command->redirs` (linked list) |

---

## Somut Örnek

```bash
cat < input.txt | grep hello > out1 > out2 > out3
```

### 1. Lexer Çıktısı (token listesi)
```
[WORD:cat] [REDIR_IN:<] [WORD:input.txt] [PIPE:|] [WORD:grep] [WORD:hello] [REDIR_OUT:>] [WORD:out1] [REDIR_OUT:>] [WORD:out2] [REDIR_OUT:>] [WORD:out3]
```

### 2. Parser Çıktısı (t_command linked list)

Parser bu token'ları pipe'lardan ayırarak iki `t_command`'a böler:

```
t_command #0                          t_command #1
┌─────────────────────┐              ┌─────────────────────────────┐
│ args: ["cat"]       │   ------>    │ args: ["grep", "hello"]     │
│ builtin: NOT_BUILTIN│   (next)     │ builtin: NOT_BUILTIN        │
│ redirs:             │              │ redirs:                     │
│   └─ REDIR_IN       │              │   ├─ REDIR_OUT "out1"      │
│      file:"input.txt"│              │   ├─ REDIR_OUT "out2"      │
│                     │              │   └─ REDIR_OUT "out3"      │
│ next: cmd#1         │              │ next: NULL                  │
└─────────────────────┘              └─────────────────────────────┘
```

> [!IMPORTANT]
> Redirectionlar **parser aşamasında** zaten `t_command->redirs` linked list'ine ekleniyor. AST bunlara dokunmuyor!

### 3. AST Çıktısı (`list_to_ast` sonrası)

`list_to_ast()` bu düz linked list'i binary tree'ye çevirir:

```
        AST_PIPE
       /        \
  AST_CMD       AST_CMD
  (cmd #0)      (cmd #1)
      │              │
      ▼              ▼
  t_command      t_command
  args:["cat"]   args:["grep","hello"]
  redirs:        redirs:
   <input.txt     >out1 → >out2 → >out3
```

---

## `list_to_ast()` Adım Adım

```c
static t_ast_node	*list_to_ast(t_sh *sh, t_command *cmd)
{
    // 1. komut yoksa NULL
    if (!cmd)
        return (NULL);

    // 2. tek komut kaldıysa, AST_CMD olarak sar ve dön
    if (!cmd->next)
        return (create_ast_cmd(sh, cmd));

    // 3. birden fazla komut var → PIPE node oluştur
    //    left  = mevcut komutu AST_CMD'ye sar
    //    right = kalanları recursive çağrıyla ağaçlaştır
    left = create_ast_cmd(sh, cmd);
    right = list_to_ast(sh, cmd->next);
    return (create_ast_pipe(sh, left, right));
}
```

### 3 komutlu örnek: `ls | grep test | wc -l`

Parser çıktısı: `cmd0(ls) → cmd1(grep test) → cmd2(wc -l)`

```
Çağrı 1: list_to_ast(cmd0)
  cmd0->next var → PIPE oluştur
  left  = AST_CMD(cmd0)        ← "ls"
  right = list_to_ast(cmd1)    ← recursive çağrı

  Çağrı 2: list_to_ast(cmd1)
    cmd1->next var → PIPE oluştur
    left  = AST_CMD(cmd1)      ← "grep test"
    right = list_to_ast(cmd2)  ← recursive çağrı

    Çağrı 3: list_to_ast(cmd2)
      cmd2->next YOK → AST_CMD(cmd2) dön  ← "wc -l"

Sonuç:
         PIPE
        /    \
    [ls]    PIPE
           /    \
     [grep test] [wc -l]
```

---

## Peki Redirectionlar Nasıl İşlenecek?

AST ağacı redirect'leri **görmüyor**. Onlar her komutun kendi içinde. Executor şöyle çalışacak:

```
executor AST'yi yürütür:
  ├── AST_PIPE görürse:
  │     pipe() çağır → fd[0] (read), fd[1] (write)
  │     fork() sol çocuk  → stdout'u fd[1]'e bağla → çalıştır
  │     fork() sağ çocuk  → stdin'i fd[0]'a bağla → çalıştır
  │
  └── AST_CMD görürse:
        cmd->redirs linked list'ini sırayla dolaş:
          REDIR_IN  "input.txt" → open() → dup2(fd, STDIN)
          REDIR_OUT "out1"      → open() → dup2(fd, STDOUT)
          REDIR_OUT "out2"      → open() → dup2(fd, STDOUT)  ← üstüne yazar
          REDIR_OUT "out3"      → open() → dup2(fd, STDOUT)  ← en son bu kazanır!
        exec komutu çalıştır
```

> [!TIP]
> **Bash davranışı**: `> out1 > out2 > out3` yazınca bash **üç dosyayı da oluşturur** (create/truncate) ama çıktı sadece **en sondaki** `out3`'e yazılır. Çünkü her `dup2()` önceki fd'yi ezer. Bu davranışı doğru taklit etmek için redirection'ları **sırayla** (linked list order) işlemen yeterli!

---

## Özet

```
                    AST NE YAPAR?
                    ─────────────
                    Pipe'ları ağaç yapar
                    Executor hangi komutun hangi komutun
                    stdout'unu alacağını bilir

                    REDİRECTION NE YAPAR?
                    ─────────────────────
                    Tek komut içinde stdin/stdout'u
                    dosyalara yönlendirir
                    Zaten t_command->redirs'te duruyor
                    AST'den bağımsız, executor'da işlenecek
```
