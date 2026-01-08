# 🎯 Lexer/Tokenizer Checklist

> **Önemli:** Bu checklist AST/Parser ile çakışmayacak şekilde düzenlenmiştir.
> Lexer'ın tek görevi: **Ham string → Token listesi** (AST ile ilgilenmez!)

---

## 📋 Öncelik Sırası (Yukarıdan Aşağıya İlerle)

### Phase 1: Temel Düzeltmeler (🔴 Kritik)

- [ ] **1.1 Quote + Text Birleşimi**
  - `"hello"world` → TEK token olmalı
  - `echo "a"'b'c` → `echo` + `"a"'b'c` (2 token)
  - Şu an: Ayrı token'lar üretiyor ❌

- [ ] **1.2 Unclosed Quote Detection**
  - `echo "hello` → Hata döndür
  - `echo 'test` → Hata döndür
  - Error handling mekanizması ekle

- [ ] **1.3 Empty Quotes**
  - `echo ""` → `echo` + `""` (boş string token'ı)
  - `echo ''` → `echo` + `''`

---

### Phase 2: Struct Güncellemeleri (🟡 Önemli)

- [ ] **2.1 Token Struct Güncelle**
  ```c
  typedef struct s_token
  {
      char    *raw;        // Orijinal: "hello"
      char    *value;      // İşlenmiş: hello (quote'suz)
      int     type;        // TOKEN_WORD, TOKEN_PIPE, vb.
      int     quote_type;  // 0=none, 1=single, 2=double, 3=mixed
      struct s_token *next;
  } t_token;
  ```

- [ ] **2.2 create_newnode() Güncelle**
  - Yeni struct alanlarını doldur

- [ ] **2.3 Error Handling Mekanizması**
  - `int *error` parametresi ekle veya
  - Result struct kullan

---

### Phase 3: Lexer Logic Refactor (🟡 Önemli)

- [ ] **3.1 Ana Lexer Döngüsü Yeniden Yaz**
  ```
  while (input[i]) {
      skip_whitespace()
      if (is_operator()) → handle_operator()
      else → handle_word()  // Quote'ları içerir
  }
  ```

- [ ] **3.2 handle_word() Fonksiyonu**
  - Quote içinde veya dışında olsun, boşluk/operator görene kadar tek token
  - Quote modunu takip et

- [ ] **3.3 Lexer State Struct (Opsiyonel)**
  ```c
  typedef struct s_lexer_state
  {
      int     pos;
      int     in_squote;
      int     in_dquote;
      char    *input;
  } t_lexer_state;
  ```

---

### Phase 4: Edge Cases (🟢 Normal)

- [ ] **4.1 Ardışık Operatörler**
  - `|||` → `|` + `|` + `|` (3 pipe)
  - `>>>` → `>>` + `>` (append + redir_out)

- [ ] **4.2 Operatör + Quote Bitişik**
  - `|"hello"` → `|` + `"hello"`
  - `>"file"` → `>` + `"file"`

- [ ] **4.3 Tab ve Diğer Whitespace**
  - `\t` (tab) de ayırıcı olmalı

---

## 🚫 Lexer'ın Yapmaması Gerekenler (AST/Parser İşi)

| Konu | Neden Lexer'da Değil |
|------|----------------------|
| `$VAR` expansion | Parser/Expander aşamasında yapılır |
| Syntax validation | `cmd1 | | cmd2` gibi hatalar Parser kontrolü |
| Command gruplandırma | AST'nin işi |
| Pipe bağlantıları | AST node'ları olarak kurulur |
| Redirection file açma | Executor'ın işi |

---

## 🔄 Lexer → AST Akışı

```
                    LEXER                           PARSER/AST
                    ─────                           ──────────
Input: "ls -la | grep foo > out.txt"
                      │
                      ▼
              ┌───────────────┐
              │ Token Listesi │
              │───────────────│
              │ WORD: ls      │
              │ WORD: -la     │
              │ PIPE: |       │──────────────▶  AST oluştur
              │ WORD: grep    │               (pipe node, cmd node, vb.)
              │ WORD: foo     │
              │ REDIR_OUT: >  │
              │ WORD: out.txt │
              └───────────────┘
```

**Lexer sadece token üretir, yapılandırmaz!**

---

## ✅ Tamamlandıkça İşaretle

Her maddeyı bitirince `[ ]` → `[x]` yap.

İyi çalışmalar! 🚀