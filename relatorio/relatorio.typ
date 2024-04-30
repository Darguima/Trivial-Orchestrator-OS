#import "resources/meta.typ" as meta
#import "resources/capa.typ" as capa

#capa.cover()

= 1. Titulo
Texto

== 1.1 Tabela
#figure(
    caption: "Tabela 1.",
    table(columns: 2)[column1][column2][row1][row2]
) <tabela1>