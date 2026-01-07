# Minishell Lexer / Tokenizer Test Scenarios

Bu dosya, Lexer modülünün "Input" -> "Expected Tokens" dönüşümlerini doğrulamak için hazırlanmıştır.

## 1. Temel Komutlar (Basic)
| Senaryo | Girdi (Input) | Beklenen Token Listesi |
| :--- | :--- | :--- |
| Basit Komut | `ls` | `[WORD: ls]` |
| Argümanlı | `ls -la` | `[WORD: ls]`, `[WORD: -la]` |
| Çoklu Argüman | `echo hello world` | `[WORD: echo]`, `[WORD: hello]`, `[WORD: world]` |

## 2. Boşluk Yönetimi (Whitespace)
| Senaryo | Girdi (Input) | Beklenen Token Listesi |
| :--- | :--- | :--- |
| Bol Boşluklu | `ls        -la` | `[WORD: ls]`, `[WORD: -la]` |
| Tab Karakteri | `echo\tmerhaba` | `[WORD: echo]`, `[WORD: merhaba]` |
| Baş/Son Boşluk | `   ls -la   ` | `[WORD: ls]`, `[WORD: -la]` |

## 3. Operatörler ve Bitişik Durumlar (Metachars)
| Senaryo | Girdi (Input) | Beklenen Token Listesi |
| :--- | :--- | :--- |
| Basit Pipe | `ls|wc` | `[WORD: ls]`, `[PIPE: |]`, `[WORD: wc]` |
| Input Redir | `cat<file` | `[WORD: cat]`, `[REDIR_IN: <]`, `[WORD: file]` |
| Output Redir | `echo h>out` | `[WORD: echo]`, `[WORD: h]`, `[REDIR_OUT: >]`, `[WORD: out]` |
| Append (Çift) | `ls>>file` | `[WORD: ls]`, `[REDIR_APPEND: >>]`, `[WORD: file]` |
| Heredoc | `cat<<EOF` | `[WORD: cat]`, `[REDIR_HEREDOC: <<]`, `[WORD: EOF]` |
| Karışık | `cat<Makefile|grep` | `[WORD: cat]`, `[REDIR_IN: <]`, `[WORD: Makefile]`, `[PIPE: |]`, `[WORD: grep]` |

## 4. Tırnak İşaretleri (Quotes)
| Senaryo | Girdi (Input) | Beklenen Token Listesi |
| :--- | :--- | :--- |
| Tek Tırnak | `echo 'hello world'` | `[WORD: echo]`, `[WORD: 'hello world']` |
| Çift Tırnak | `echo "hello world"` | `[WORD: echo]`, `[WORD: "hello world"]` |
| Pipe Tırnakta | `echo "|"` | `[WORD: echo]`, `[WORD: "|"]` |
| Boş String | `echo ""` | `[WORD: echo]`, `[WORD: ""]` |
| Nested | `echo "'test'"` | `[WORD: echo]`, `[WORD: "'test'"]` |

## 5. Zorlayıcı Durumlar (Edge Cases)
| Senaryo | Girdi (Input) | Beklenen Token Listesi | Not |
| :--- | :--- | :--- | :--- |
| Yapışık Quote | `echo "Hel"lo` | `[WORD: echo]`, `[WORD: "Hel"lo]` | Boşluksuz |
| Operator Karışık | `echo >>> out` | `[WORD: echo]`, `[APPEND: >>]`, `[OUT: >]`, `[WORD: out]` | Greedy match |
| Quote İçi Quote | `echo " '" ` | `[WORD: echo]`, `[WORD: " '"]` | Literal kabul edilir |

## 6. Hata Durumları (Errors)
Lexer bu durumlarda hata döndürmeli veya prompt'u tekrar göstermelidir:
* `echo "unclosed` (Kapanmamış çift tırnak)
* `echo 'unclosed` (Kapanmamış tek tırnak)
* `\` veya `;` (Bonus yapılmıyorsa desteklenmeyen karakterler)

## 📝 Minishell Lexer To-Do List
Bu liste, ham string'i (input) alıp tertemiz bir Token Linked List'e çevirene kadar yapman gerekenleri adım adım içerir.




### 🛠 Bölüm 1: Hazırlık ve Yapı (Setup)
Temeli sağlam atmazsak bina çöker.

[ ] Token Enum Tanımla:

Token tiplerini belirle (TOKEN_WORD, TOKEN_PIPE, TOKEN_REDIR_IN, TOKEN_REDIR_OUT, TOKEN_HEREDOC vb.).

[ ] Token Struct Tasarla:

İçinde content (char *), type (enum), state (quote durumu) ve next (struct pointer) olsun.

[ ] Global/Main Struct Güncelle:

Lexer'ın sonucunu tutacak olan listenin başını (t_token *head) ana yapıya ekle.

### 🔍 Bölüm 2: Yardımcı Fonksiyonlar (Helpers)
Ana döngüyü kirletmemek için bu askerleri önden yaz.

[ ] is_whitespace(char c):

Karakter boşluk veya tab mı? (Evet/Hayır döndür).

[ ] is_separator(char c):

Karakter bir ayırıcı mı? (|, <, >, ). Tırnak içindeyse ayırıcı sayılmamalı, buna dikkat.

[ ] is_quote(char c):

Karakter ' veya " mi?

[ ] check_quote_state(...):

Bu çok kritik. Şu an hangi tırnak durumundayım? (DEFAULT, SQUOTE, DQUOTE). Her karakterde bu durumu güncelleyen bir lojiğin olmalı.

### ⚙️ Bölüm 3: Ana Motor (The Core Logic)
String'in üzerinde yürüyen o meşhur while döngüsü.

[ ] Whitespace Atlayıcı (Skip Whitespace):

Döngü başlar başlamaz boşlukları geç (i++).

[ ] Operatör Yakalayıcı (Handle Operators):

Eğer karakter |, < veya > ise:

Tekli mi (>) yoksa çiftli mi (>>) kontrol et.

Bunu bir token yapıp listeye ekle.

İndeksi (i) ilerlet.

[ ] Kelime Yakalayıcı (Handle Words):

Eğer karakter operatör veya boşluk değilse:

Burada bir "Kelime Başlangıcı" var.

Bir iç döngü ile "Kelime Bitişi"ni bul (Bir sonraki boşluk veya operatöre kadar git, AMA tırnak içindeyken durma!).

Başlangıç ve bitiş arasındaki string'i kopyala (ft_substr benzeri).

Token yapıp listeye ekle.

[ ] Tırnak Kontrolü (Error Check):

Satır bittiğinde hala state değişkenin tırnak içinde kaldıysa hata döndür.

### 🔗 Bölüm 4: Liste Yönetimi (Linked List Utils)
Listeye ekleme çıkarma işleri.

[ ] token_new(content, type):

Yeni bir düğüm oluştur, malloc ile yer ayır, değerleri ata.

[ ] token_add_back(**head, *new):

Listenin sonuna yeni düğümü ekle.

[ ] token_clear(**head):

Çok Önemli: İş bittiğinde veya hata aldığında tüm listeyi ve içindeki content'leri freeleyen fonksiyon. (Memory Leak istemeyiz!).

### 🧪 Bölüm 5: Test ve Entegrasyon
Yaptıklarımız çalışıyor mu?

[ ] Debug Printer:

Listeyi baştan sona gezip Type ve Content değerlerini ekrana basan geçici bir fonksiyon yaz.

[ ] Input Testleri:

Sana verdiğim .md dosyasındaki senaryoları tek tek dene.

[ ] Valgrind Kontrolü:

exit yazdığında veya Ctrl+D yaptığında arkada still reachable dışında leak kalmamalı.