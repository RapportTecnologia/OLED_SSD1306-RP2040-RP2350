# OLED SSD1306 (Proxy `oled.h/.c`)

Projeto para uso do display OLED SSD1306 via I2C no Raspberry Pi Pico (ou compatíveis), oferecendo uma camada de alto nível (proxy/fachada) em `oled.h`/`oled.c` sobre o driver de baixo nível em `ssd1306_*`. Essa camada simplifica operações comuns como inicialização, limpeza, desenho de pixels/linhas, e renderização de textos (com alinhamento e fonte grande).

## Como obter este módulo (via workspace principal)

Para garantir as dependências e caminhos relativos esperados pelos exemplos/projetos (quando usados em conjunto com outros diretórios do workspace), recomenda-se clonar o repositório raiz (monorepo) com submódulos:

```bash
git clone --recurse-submodules https://github.com/RapportTecnologia/Sensores_e_Atuadores_do_Kit_BitDogLab.git Sensores_e_Atuadores_do_KIT
cd Sensores_e_Atuadores_do_KIT
git submodule update --init --recursive
```

Atualizar submódulos (quando aplicável):

```bash
git submodule update --remote --merge --recursive
```

## Visão Geral

- __Objetivo__: disponibilizar uma API simples para desenhar e compor telas sem lidar diretamente com comandos do SSD1306.
- __Abordagem__: o proxy mantém um buffer de vídeo em RAM e só envia ao display quando você chama `oled_render()` (ou utiliza `oled_render_text()` para o modo de texto).
- __Display suportado__: 128x64, I2C endereço padrão `0x3C`.

## Estrutura do Projeto

- `oled.h` / `oled.c`: proxy/fachada de alto nível (API pública recomendada).
- `ssd1306_i2c.h` / `ssd1306_i2c.c`: driver e primitivas de desenho/transferência I2C.
- `ssd1306.h`: protótipos e integração com o driver.
- `ssd1306_font.h`: fonte padrão (8x8) e utilitários.
- `big_font.h`: fonte grande para números e caracteres destacados.

## Requisitos e Hardware

- __SDK__: Raspberry Pi Pico SDK (Pico SDK) e toolchain C/C++ configurados.
- __I2C__: clock padrão 400 kHz (`ssd1306_i2c_clock 400`).
- __Pinos I2C padrão__ (podem ser alterados em `oled.h`):
  - `I2C_SDA` = 14
  - `I2C_SCL` = 15
- __Endereço I2C__: `0x3C` (definido em `ssd1306_i2c.h` como `ssd1306_i2c_address`).

Faça a ligação do módulo OLED (VCC, GND, SDA, SCL) aos pinos I2C do Pico. Se utilizar outros pinos, ajuste os `#define` em `oled.h`.

## API do Proxy (`oled.h`)

- `void oled_init(void);`
  - Inicializa I2C, configura o SSD1306 e limpa o buffer interno.

- `void oled_clear(void);`
  - Limpa o buffer de vídeo em RAM e atualiza o display imediatamente.

- `void oled_render(void);`
  - Envia o conteúdo do buffer atual ao display (uso após desenhar).

- `void oled_set_pixel(int x, int y, bool on);`
  - Define um pixel no buffer (não envia automaticamente ao display).

- `void oled_draw_line(int x0, int y0, int x1, int y1, bool on);`
  - Desenha uma linha no buffer.

- `void oled_draw_char(int x, int y, char c);`
  - Desenha um caractere (fonte padrão, largura 8).

- `void oled_draw_string(int x, int y, const char *str);`
  - Desenha uma string (fonte padrão). Altura de linha: `ssd1306_line_height` (8 px).

- `void oled_draw_big_char(int x, int y, char c);`
  - Desenha caractere usando a fonte grande (`big_font.h`).

- Modo de “buffer de texto” (linhas lógicas):
  - `typedef enum { OLED_ALIGN_LEFT, OLED_ALIGN_CENTER, OLED_ALIGN_RIGHT, OLED_ALIGN_JUSTIFY } oled_text_alignment_t;`
  - `void oled_set_text_line(uint8_t line, const char *text, oled_text_alignment_t alignment);`
  - `void oled_clear_text_line(uint8_t line);`
  - `void oled_render_text(void);`

Parâmetros relevantes (em `ssd1306_i2c.h`):

- `ssd1306_width = 128`, `ssd1306_height = 64`.
- `ssd1306_line_height = 8`.
- `max_text_lines = 8` e `max_text_columns = 17` (para o modo de texto do proxy).
- `font_width = 8` (padrão, pode ser redefinido antes de incluir `oled.h`).

## Fluxo de Uso Recomendado

1. __Inicialize__: `oled_init();`
2. __Desenhe no buffer__: use `oled_set_pixel`, `oled_draw_line`, `oled_draw_char`/`oled_draw_string`.
3. __Renderize__: chame `oled_render();` para enviar ao display.

Para o modo de texto por linhas:

1. Chame `oled_set_text_line(line, "texto", OLED_ALIGN_...);` para cada linha desejada (0 a `max_text_lines-1`).
2. Chame uma única vez `oled_render_text();` para compor e enviar o conteúdo.

## Exemplo Rápido (Desenho livre)

```c
#include "oled.h"

int main() {
    oled_init();

    // Limpa tela
    oled_clear();

    // Desenha uma moldura
    oled_draw_line(0, 0, 127, 0, true);
    oled_draw_line(0, 63, 127, 63, true);
    oled_draw_line(0, 0, 0, 63, true);
    oled_draw_line(127, 0, 127, 63, true);

    // Escreve um texto
    oled_draw_string(8, 8, "OLED SSD1306");

    // Renderiza
    oled_render();

    while (1) { /* loop */ }
}
```

## Exemplo (Modo de texto com alinhamento)

```c
#include "oled.h"

int main() {
    oled_init();

    oled_set_text_line(0, "Sistema", OLED_ALIGN_LEFT);
    oled_set_text_line(1, "Pico + OLED", OLED_ALIGN_CENTER);
    oled_set_text_line(2, "Status: OK", OLED_ALIGN_RIGHT);

    // Justify é tratado como LEFT no momento
    oled_set_text_line(3, "Justify demo", OLED_ALIGN_JUSTIFY);

    oled_render_text();
    while (1) {}
}
```

## Exemplo (Fonte grande)

```c
#include "oled.h"

int main() {
    oled_init();
    oled_clear();

    oled_draw_big_char(0, 0, '2');
    oled_draw_big_char(20, 0, '5');
    oled_draw_big_char(40, 0, 'C');

    oled_render();
    while (1) {}
}
```

## Boas Práticas para Melhor Resultado com o Proxy

- __Desenhe em lote, renderize uma vez__: acumule alterações no buffer e chame `oled_render()` no final do quadro/tela. Evite renderizações parciais repetidas.
- __Use o modo de texto quando fizer sentido__: `oled_set_text_line()` + `oled_render_text()` já cuidam do alinhamento e composição vertical por linhas de 8 px.
- __Valide coordenadas__: mantenha `x` em `[0, 127]` e `y` em `[0, 63]` para evitar recortes inesperados.
- __Fonte e limites__: no modo de texto, `max_text_columns` controla o truncamento; evite strings maiores que 17 caracteres ou ajuste o valor para seu caso.
- __I2C estável__: mantenha o clock em 400 kHz para boa taxa de atualização. Se houver ruído, revisite fiação e pull-ups (o código aplica `gpio_pull_up`).
- __Pinos customizados__: altere `I2C_SDA` e `I2C_SCL` em `oled.h` conforme seu hardware.
- __Limpeza eficiente__: `oled_clear()` zera o buffer e envia ao display; se estiver no modo de texto, `oled_render_text()` já chama `oled_clear()` internamente antes de compor.
- __JUSTIFY__: atualmente é tratado como LEFT; se precisar de justificação real entre palavras, considere implementar a distribuição de espaços no `oled_render_text()`.

## Limitações e Observações

- Otimizações de renderização parcial não estão habilitadas; cada `oled_render()` envia todo o buffer.
- O modo de texto usa `font_width = 8`. Se alterar a fonte, ajuste alinhamento e limites para evitar cortes.
- `max_text_lines` e `max_text_columns` são definidos em `ssd1306_i2c.h`. Ajustes podem exigir revisitar o layout.

## Licença

Consulte `LICENSE` neste diretório.
