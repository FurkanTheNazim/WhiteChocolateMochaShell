# 🏗️ PARSER TODO - Kallavi Edition

> **Proje:** WhiteChocolateMochaShell  
> **Son Güncelleme:** 2026-01-26  
> **Hedef:** Token listesini çalıştırılabilir komut yapılarına dönüştürmek

---

## 🎯 Parser'ın Görevi

```mermaid
graph LR
    A[Token Listesi] --> B[Parser]
    B --> C[Command Listesi]
    C --> D[Executor]
```

**Girdi:** `t_token` linked list (lexer'dan)  
**Çıktı:** `t_command` linked list (executor'a)

---

## 📁 Dosya Yapısı

```
src/parser/
├── parser.c           # [ ] Ana parser fonksiyonu
├── parser_utils.c     # [ ] Yardımcı fonksiyonlar
├── parser_command.c   # [ ] Command struct oluşturma
├── parser_redir.c     # [ ] Redirection handling
└── parser_validate.c  # [ ] Syntax validation
```

---

## 📋 PHASE 0: Struct Tanımlamaları

### 0.1 Command Struct (`lexer_parser.h`'a eklenecek)
```c
typedef struct s_redir
{
    int             type;       // TOKEN_REDIR_IN, TOKEN_REDIR_OUT, etc.
    char            *file;      // Dosya adı veya heredoc delimiter
    struct s_redir  *next;      // Birden fazla redir olabilir
}   t_redir;

typedef struct s_command
{
    char                **args;     // ["ls", "-la", NULL]
    t_redir             *redirs;    // Tüm redirection'lar
    struct s_command    *next;      // Pipe sonrası komut
}   t_command;
```

### 0.2 Checklist
- [ ] `t_redir` struct'ını `lexer_parser.h`'a ekle
- [ ] `t_command` struct'ını `lexer_parser.h`'a ekle
- [ ] `t_minishell`'e `t_command *cmd_list` ekle

---

## 📋 PHASE 1: Altyapı

### 1.1 Command Yardımcı Fonksiyonları (`parser_command.c`)
- [ ] `t_command *create_command(void)` — Boş command oluştur
- [ ] `void add_arg(t_command *cmd, char *arg)` — Argüman ekle
- [ ] `void add_command(t_command **list, t_command *new)` — Listeye ekle
- [ ] `void free_command_list(t_command *list)` — Memory temizliği

### 1.2 Redir Yardımcı Fonksiyonları (`parser_redir.c`)
- [ ] `t_redir *create_redir(int type, char *file)` — Redir oluştur
- [ ] `void add_redir(t_command *cmd, t_redir *redir)` — Komuta redir ekle
- [ ] `void free_redir_list(t_redir *list)` — Memory temizliği

### 1.3 Parser Utils (`parser_utils.c`)
- [ ] `int is_redir_token(int type)` — Redir mi kontrol et
- [ ] `t_token *skip_to_pipe(t_token *token)` — Pipe'a atla
- [ ] `int count_args(t_token *start, t_token *end)` — Argüman sayısı

---

## 📋 PHASE 2: Ana Parser Logic (`parser.c`)

### 2.1 Ana Parser Fonksiyonu
```c
t_command *parser(t_token *tokens, int *error)
{
    // 1. Token listesini pipe'lara göre böl
    // 2. Her segment için:
    //    a. Redirection'ları ayır ve t_redir listesine ekle
    //    b. Kalan WORD'leri args[] array'ine koy
    // 3. Command'ları linked list olarak bağla
    // 4. return cmd_list
}
```

### 2.2 Checklist
- [ ] `t_command *parser(t_token *tokens, int *error)` — Ana fonksiyon
- [ ] `t_command *parse_simple_command(t_token **tokens)` — Tek komut parse et
- [ ] `int handle_redirection(t_token **tokens, t_command *cmd)` — Redir işle

---

## 📋 PHASE 3: Syntax Validation (`parser_validate.c`)

### 3.1 Kontrol Edilecek Hatalar
- [ ] `| |` — Peş peşe pipe (syntax error)
- [ ] `|` ile başlama (syntax error)
- [ ] `|` ile bitme (syntax error)  
- [ ] `< >` — Redir sonrası dosya yok (syntax error)
- [ ] `> >` — Peş peşe redir (syntax error)
- [ ] `<<` sonrası delimiter yok (syntax error)

### 3.2 Fonksiyonlar
- [ ] `int validate_tokens(t_token *tokens)` — Tüm validasyonları çalıştır
- [ ] `int check_pipe_syntax(t_token *tokens)` — Pipe kontrolü
- [ ] `int check_redir_syntax(t_token *tokens)` — Redir kontrolü

---

## 📋 PHASE 4: Quote Stripping (Opsiyonel - Expander'da da yapılabilir)

### 4.1 Quote Temizleme
- [ ] `"hello"` → `hello`
- [ ] `'world'` → `world`
- [ ] `"hello"world` → `helloworld`

### 4.2 Fonksiyonlar
- [ ] `char *strip_quotes(char *str, int quote_type)` — Quote'ları sil

> [!NOTE]
> Quote stripping, expansion'dan ÖNCE mi SONRA mi yapılmalı? 
> Bash'te önce expansion, sonra quote removal yapılır.
> Ama single quote içindeki `$VAR` expand EDİLMEZ!

---

## 📋 PHASE 5: Test Cases

### 5.1 Basit Komutlar
```bash
ls                    → cmd: ["ls", NULL], redir: NULL
ls -la                → cmd: ["ls", "-la", NULL], redir: NULL
echo hello world      → cmd: ["echo", "hello", "world", NULL], redir: NULL
```

### 5.2 Redirection Testleri
```bash
ls > file             → cmd: ["ls", NULL], redir: {REDIR_OUT, "file"}
cat < input           → cmd: ["cat", NULL], redir: {REDIR_IN, "input"}
cat < in > out        → cmd: ["cat", NULL], redir: [{REDIR_IN, "in"}, {REDIR_OUT, "out"}]
echo hi >> file       → cmd: ["echo", "hi", NULL], redir: {REDIR_APPEND, "file"}
cat << EOF            → cmd: ["cat", NULL], redir: {HEREDOC, "EOF"}
```

### 5.3 Pipe Testleri
```bash
ls | cat              → cmd1: ["ls"], cmd2: ["cat"], pipe bağlı
ls | cat | wc         → 3 command, birbirine bağlı
ls -la | grep txt     → cmd1: ["ls", "-la"], cmd2: ["grep", "txt"]
```

### 5.4 Karmaşık Testler
```bash
< in cat > out                → cmd: ["cat"], redir: [{IN, "in"}, {OUT, "out"}]
ls -la | grep a > out.txt     → cmd1: ["ls", "-la"], cmd2: ["grep", "a"], redir: {OUT, "out.txt"}
cat << EOF | grep hi > out    → Heredoc + pipe + redir
```

### 5.5 Syntax Error Testleri
```bash
|                     → ERROR: syntax error near '|'
| ls                  → ERROR: syntax error near '|'
ls |                  → ERROR: syntax error near newline
ls | |                → ERROR: syntax error near '|'
ls >                  → ERROR: syntax error near newline
ls > > file           → ERROR: syntax error near '>'
< < file              → ERROR: syntax error near '<'
```

---

## 🔄 Parser Akışı (Örnek)

### Girdi: `ls -la | grep txt > out.txt`

**Step 1: Lexer Çıktısı (Token List)**
```
[WORD:ls] → [WORD:-la] → [PIPE:|] → [WORD:grep] → [WORD:txt] → [REDIR_OUT:>] → [WORD:out.txt]
```

**Step 2: Pipe'a Göre Böl**
```
Segment 1: [WORD:ls] → [WORD:-la]
Segment 2: [WORD:grep] → [WORD:txt] → [REDIR_OUT:>] → [WORD:out.txt]
```

**Step 3: Her Segment'i Parse Et**
```
Command 1:
  args: ["ls", "-la", NULL]
  redirs: NULL
  next: → Command 2

Command 2:
  args: ["grep", "txt", NULL]
  redirs: [{REDIR_OUT, "out.txt"}]
  next: NULL
```

---

## 🚫 PARSER'IN YAPMAYACAĞI ŞEYLER

| Görev | Kim Yapacak? |
|-------|--------------|
| `$VAR` expansion | Expander |
| Dosya açma/oluşturma | Executor |
| Fork/exec | Executor |
| Pipe oluşturma | Executor |
| Heredoc okuma | Executor (veya ayrı modül) |
| Builtin çalıştırma | Executor |

---

## 🏁 İlerleme Durumu

```
PHASE 0: ░░░░░░░░░░   0%  Struct Tanımlamaları
PHASE 1: ░░░░░░░░░░   0%  Altyapı
PHASE 2: ░░░░░░░░░░   0%  Ana Parser Logic
PHASE 3: ░░░░░░░░░░   0%  Syntax Validation
PHASE 4: ░░░░░░░░░░   0%  Quote Stripping
PHASE 5: ░░░░░░░░░░   0%  Testler
─────────────────────────
TOPLAM:  ░░░░░░░░░░   0%
```

---

## 📝 Önemli Notlar

### Redir Pozisyonu
Bash'te redirection komutun HERHANGİ BİR yerinde olabilir:
```bash
> out ls -la          # Geçerli!
ls > out -la          # Geçerli!
ls -la > out          # Geçerli!
```
Hepsi aynı anlama gelir. Parser bunu handle etmeli!

### Birden Fazla Redirection
```bash
cat < in1 < in2       # Son olan geçerli (in2)
cat > out1 > out2     # Son olan geçerli (out2)
cat < in > out        # İkisi de geçerli
```

### Heredoc Özel Durumu
```bash
cat << EOF
hello
world
EOF
```
Heredoc delimiter'ı (`EOF`) parse edilmeli, ama içerik **executor'da** okunacak!

---

## 🔧 Header'a Eklenecekler

```c
// lexer_parser.h'a ekle:

// Redirection struct
typedef struct s_redir
{
    int             type;
    char            *file;
    struct s_redir  *next;
}   t_redir;

// Command struct
typedef struct s_command
{
    char                **args;
    t_redir             *redirs;
    struct s_command    *next;
}   t_command;

// Parser fonksiyonları
t_command   *parser(t_token *tokens, int *error);
int         validate_tokens(t_token *tokens);
void        free_command_list(t_command *list);
```

---

> **Kural:** İlk önce struct'ları tanımla, sonra test yaz, en son implement et! 🚀
