# 🎯 LEXER TODO - Kallavi Edition

> **Proje:** WhiteChocolateMochaShell  
> **Son Güncelleme:** 2026-01-26  
> **Hedef:** Bash-uyumlu lexer/tokenizer

---

## 📁 Dosya Yapısı

```
src/lexer/
├── lexer.c           # [x] Ana lexer fonksiyonu
├── lexer_utils.c     # [x] Yardımcı fonksiyonlar  
├── lexer_operators.c # [x] Operatör handling
└── lexer_token.c     # [x] Token oluşturma
```

---

## 📋 PHASE 1: Altyapı ✅ TAMAMLANDI

### 1.1 Token Yardımcı Fonksiyonları (`lexer_token.c`)
- [x] `t_token *create_newnode(char *raw, t_lexer_state state, char *token)`
- [x] `void addback(t_minishell *shell, t_token *newnode)`
- [x] `void free_node(t_token **list)`
- [ ] `void print_tokens(t_token *list)` — debug için (main.c'de inline yapılıyor)

### 1.2 Utility Fonksiyonları (`lexer_utils.c`)
- [x] `int ft_isspace(char c)` — ' ' ve '\t' kontrolü
- [x] `int is_operator(char c)` — |, <, > kontrolü
- [x] `void init_lexer_stat(t_lexer_state *state)`
- [ ] `int is_quote(char c)` — ', " kontrolü (inline yapılıyor)
- [ ] `char *extract_value(char *raw, int quote_type)` — Quote'ları sil (parser'da yapılacak)

---

## 📋 PHASE 2: Operatör Handling ✅ TAMAMLANDI

### 2.1 Temel Operatörler (`lexer_operators.c`)
- [x] `int append_operator(t_lexer_state state, char *ptr, t_minishell *shell)`
  - [x] `|` → TOKEN_PIPE
  - [x] `||` → TOKEN_OR
  - [x] `<` → TOKEN_REDIR_IN
  - [x] `>` → TOKEN_REDIR_OUT
  - [x] `<<` → TOKEN_HEREDOC
  - [x] `>>` → TOKEN_REDIR_APPEND

### 2.2 Edge Cases
- [x] `||` → OR operatörü
- [x] `>>` → append operatörü
- [x] `<<` → heredoc operatörü
- [ ] `|||` → test edilmeli
- [ ] `>>>` → test edilmeli

---

## 📋 PHASE 3: Quote Handling ✅ TAMAMLANDI

### 3.1 Single Quote (`lexer.c` - `handle_single_quote`)
- [x] `'hello'` → value: 'hello', quote_type: S_QUOTE
- [x] `''` → value: (empty), quote_type: S_QUOTE
- [x] `'hello` → **HATA** (unclosed quote) ✅

### 3.2 Double Quote (`lexer.c` - `handle_double_quote`)
- [x] `"hello"` → value: "hello", quote_type: D_QUOTE
- [x] `""` → value: (empty), quote_type: D_QUOTE
- [x] `"hello` → **HATA** (unclosed quote) ✅

### 3.3 Mixed/Bitişik Quotes
- [x] `"hello"world` → **TEK TOKEN** ✅
- [x] `'a'"b"c` → **TEK TOKEN** ✅
- [x] `"it's"` → value: "it's" ✅

---

## 📋 PHASE 4: Ana Lexer Logic ✅ TAMAMLANDI

### 4.1 handle_word() Fonksiyonu (`lexer.c`)
- [x] Boşluk/operator görene kadar oku
- [x] Quote içindeyken boşluk/operator'ü dikkate alma
- [x] Nested quote handling:
  - [x] `"hello"` → tek token
  - [x] `"hello"world` → tek token (bitişik)
  - [x] `hello world` → iki token

### 4.2 Ana lexer() Döngüsü (`lexer.c`)
```c
t_token *lexer(t_minishell *shell)
{
    // 1. state init ✅
    // 2. while (shell->input[i]) ✅
    //    - skip_whitespace ✅
    //    - if operator → append_operator ✅
    //    - else → handle_word (quote dahil) ✅
    // 3. return token_list ✅
}
```

---

## 📋 PHASE 5: Error Handling ✅ TAMAMLANDI

### 5.1 Syntax Errors (`lexer.c` - `handle_syntax_error`)
- [x] Unclosed single quote → return -1
- [x] Unclosed double quote → return -1
- [x] Error mesajı: `"minishell: syntax error: unclosed quote"`
- [x] `shell->syntax_error = 1` flag'i set ediliyor

### 5.2 Memory Errors
- [x] malloc fail → NULL döndür
- [x] `free_node()` fonksiyonu mevcut

---

## 📋 PHASE 6: Test Cases

### 6.1 Temel Testler ✅
```bash
echo hello              → [echo] [hello] ✅
ls -la                  → [ls] [-la] ✅
cat file.txt            → [cat] [file.txt] ✅
```

### 6.2 Quote Testleri ✅
```bash
echo "hello world"      → [echo] ["hello world"] ✅
echo 'test'             → [echo] ['test'] ✅
echo "hello"world       → [echo] ["hello"world] ✅
echo ""                 → [echo] [""] ✅
```

### 6.3 Operator Testleri
```bash
ls | cat                → [ls] [|] [cat] (test edilmeli)
cat < in > out          → [cat] [<] [in] [>] [out] (test edilmeli)
cat << EOF              → [cat] [<<] [EOF] (test edilmeli)
echo hi >> file         → [echo] [hi] [>>] [file] (test edilmeli)
```

### 6.4 Karmaşık Testler
```bash
echo "hello"world$PATH  → Test edilmeli
ls|cat                  → Test edilmeli (boşluksuz)
"hello"'world'          → Test edilmeli (mixed quotes)
```

### 6.5 Error Testleri ✅
```bash
echo "hello             → ERROR: unclosed quote ✅
echo 'test              → ERROR: unclosed quote ✅
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
PHASE 1: ██████████ 100%  Altyapı ✅
PHASE 2: ██████████ 100%  Operatörler ✅
PHASE 3: ██████████ 100%  Quotes ✅
PHASE 4: ██████████ 100%  Ana Lexer ✅
PHASE 5: ██████████ 100%  Error Handling ✅
PHASE 6: ████░░░░░░  40%  Testler (bazı edge case'ler test edilmeli)
─────────────────────────
TOPLAM:  █████████░  90%
```

---

## 📝 Sonraki Adımlar

1. **Parser Modülü** → Token listesini AST'ye çevir
2. **Expander** → `$VAR` ve `$?` expansion
3. **Executor** → Komutları çalıştır
4. **Builtins** → `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

---

## 🔧 Mevcut Implementasyon Detayları

### Struct'lar (`WCMS.h`)
- `t_lexer_state` → Quote state tracking ✅
- `t_token` → Token linked list ✅
- `t_minishell` → Shell state ✅

### Token Types
```c
TOKEN_WORD        // Normal word
TOKEN_PIPE        // |
TOKEN_OR          // ||
TOKEN_REDIR_IN    // <
TOKEN_REDIR_OUT   // >
TOKEN_HEREDOC     // <<
TOKEN_REDIR_APPEND // >>
```

### Quote Types
```c
N_QUOTE  // No quote
S_QUOTE  // Single quote '
D_QUOTE  // Double quote "
```

---

> **Durum:** LEXER TAMAMLANDI 🎉 Sırada PARSER var!