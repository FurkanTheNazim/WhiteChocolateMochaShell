# 🎯 LEXER TODO - Kallavi Edition

> **Proje:** WhiteChocolateMochaShell  
> **Son Güncelleme:** 2026-01-08  
> **Hedef:** Bash-uyumlu lexer/tokenizer

---

## 📁 Dosya Yapısı

```
src/lexer/
├── lexer.c           # [/] Ana lexer fonksiyonu
├── lexer_utils.c     # [ ] Yardımcı fonksiyonlar  
├── lexer_operators.c # [ ] Operatör handling
├── lexer_quotes.c    # [ ] Quote handling (oluşturulacak)
└── lexer_token.c     # [ ] Token oluşturma (oluşturulacak)
```

---

## 📋 PHASE 1: Altyapı (Önce Bunları Yap)

### 1.1 Token Yardımcı Fonksiyonları (`lexer_token.c`)
- [ ] `t_token *create_token(char *raw, char *value, int type, int quote_type)`
- [ ] `void add_token(t_token **list, t_token *new_token)`
- [ ] `void free_token_list(t_token *list)`
- [ ] `void print_tokens(t_token *list)` — debug için

### 1.2 Utility Fonksiyonları (`lexer_utils.c`)
- [ ] `int is_whitespace(char c)` — ' ' ve '\t' kontrolü
- [ ] `int is_operator_char(char c)` — |, <, > kontrolü
- [ ] `int is_quote(char c)` — ', " kontrolü
- [ ] `void skip_whitespace(t_lexer_state *state)`
- [ ] `char *extract_value(char *raw, int quote_type)` — Quote'ları sil

---

## 📋 PHASE 2: Operatör Handling (`lexer_operators.c`)

### 2.1 Temel Operatörler
- [ ] `int handle_operator(t_lexer_state *state, t_token **list)`
  - [ ] `|` → TOKEN_PIPE
  - [ ] `<` → TOKEN_REDIR_IN
  - [ ] `>` → TOKEN_REDIR_OUT
  - [ ] `<<` → TOKEN_HEREDOC
  - [ ] `>>` → TOKEN_REDIR_APPEND

### 2.2 Edge Cases
- [ ] `|||` → 3 ayrı pipe token
- [ ] `>>>` → `>>` + `>` (append + redir_out)
- [ ] `|>` → pipe + redir_out

---

## 📋 PHASE 3: Quote Handling (`lexer_quotes.c`)

### 3.1 Single Quote
- [ ] `'hello'` → value: hello, quote_type: 1
- [ ] `''` → value: (empty), quote_type: 1
- [ ] `'hello` → **HATA** (unclosed quote)

### 3.2 Double Quote
- [ ] `"hello"` → value: hello, quote_type: 2
- [ ] `""` → value: (empty), quote_type: 2
- [ ] `"hello` → **HATA** (unclosed quote)

### 3.3 Mixed/Bitişik Quotes
- [ ] `"hello"world` → **TEK TOKEN**, quote_type: 3 (mixed)
- [ ] `'a'"b"c` → **TEK TOKEN**
- [ ] `"it's"` → value: it's

---

## 📋 PHASE 4: Ana Lexer Logic (`lexer.c`)

### 4.1 handle_word() Fonksiyonu
- [ ] Boşluk/operator görene kadar oku
- [ ] Quote içindeyken boşluk/operator'ü dikkate alma
- [ ] Nested quote handling:
  - [ ] `"hello"` → tek token
  - [ ] `"hello"world` → tek token (bitişik)
  - [ ] `hello world` → iki token

### 4.2 Ana lexer() Döngüsü
```c
t_token *lexer(char *input, int *error)
{
    // 1. state init
    // 2. while (input[state.i])
    //    - skip_whitespace
    //    - if operator → handle_operator
    //    - else → handle_word (quote dahil)
    // 3. return token_list
}
```

---

## 📋 PHASE 5: Error Handling

### 5.1 Syntax Errors
- [ ] Unclosed single quote → error = 1
- [ ] Unclosed double quote → error = 2
- [ ] Error mesajı döndür: `"syntax error: unclosed quote"`

### 5.2 Memory Errors
- [ ] malloc fail → NULL döndür
- [ ] Önceki token'ları free'le

---

## 📋 PHASE 6: Test Cases

### 6.1 Temel Testler
```bash
echo hello              → [echo] [hello]
ls -la                  → [ls] [-la]
cat file.txt            → [cat] [file.txt]
```

### 6.2 Quote Testleri
```bash
echo "hello world"      → [echo] ["hello world"]
echo 'test'             → [echo] ['test']
echo "hello"world       → [echo] ["hello"world]  # TEK TOKEN!
echo ""                 → [echo] [""]
```

### 6.3 Operator Testleri
```bash
ls | cat                → [ls] [|] [cat]
cat < in > out          → [cat] [<] [in] [>] [out]
cat << EOF              → [cat] [<<] [EOF]
echo hi >> file         → [echo] [hi] [>>] [file]
```

### 6.4 Karmaşık Testler
```bash
echo "hello"world$PATH  → [echo] ["hello"world$PATH]
ls|cat                  → [ls] [|] [cat]  # Boşluksuz
"hello"'world'          → ["hello"'world']  # Mixed quotes
```

### 6.5 Error Testleri
```bash
echo "hello             → ERROR: unclosed quote
echo 'test              → ERROR: unclosed quote
```

---

## 🚫 LEXER'IN YAPMAYACAĞI ŞEYLER (AST/Parser İşi)

| Görev | Kim Yapacak? |
|-------|--------------|
| $VAR expansion | Expander |
| Syntax validation (cmd | | cmd) | Parser |
| Command gruplandırma | AST |
| Pipe/redir bağlantıları | AST nodes |
| Dosya açma | Executor |

---

## 🏁 İlerleme Durumu

```
PHASE 1: ░░░░░░░░░░  0%  Altyapı
PHASE 2: ░░░░░░░░░░  0%  Operatörler
PHASE 3: ░░░░░░░░░░  0%  Quotes
PHASE 4: ░░░░░░░░░░  0%  Ana Lexer
PHASE 5: ░░░░░░░░░░  0%  Error Handling
PHASE 6: ░░░░░░░░░░  0%  Testler
─────────────────────────
TOPLAM:  ░░░░░░░░░░  0%
```

---

## 📝 Notlar

- `t_lexer_state` struct'ı hazır ✅
- `t_token` struct'ı hazır ✅
- main() test için hazır (güncellenmeli)

---

> **Kural:** Yukarıdan aşağıya git, atla yapma! 🚀