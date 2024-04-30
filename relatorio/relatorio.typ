#import "resources/report.typ" as report

#show: report.styling.with(
    hasFooter: false
)

= Titulo
Texto

== Tabela
#figure(
    caption: "Tabela 1.",
    table(columns: 2)[column1][column2][row1][row2]
) <tabela1>

#pagebreak()

= Titulo 2
Texto

== Tabela 2
#figure(
    caption: "Tabela 2.",
    table(columns: 2)[column1][column2][row1][row2]
) <tabela2>