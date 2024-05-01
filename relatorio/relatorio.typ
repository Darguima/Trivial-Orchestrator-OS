#import "resources/report.typ" as report

#show: report.styling.with(
    hasFooter: false
)

#report.index()

= Comandos
Texto

== Cliente
=== Command Sender
Texto

== Servidor
=== Command Interpreter
Texto

=== Command Runner
Texto

= Pipes
Texto

== Cliente
Texto

== Servidor
Texto

= Escalonamento de Tarefas
Texto

== Servidor
Texto

#pagebreak()

= Titulo
Texto

== Tabela
#figure(
    caption: "Tabela 1.",
    table(columns: 2)[column1][column2][row1][row2]
) <tabela1>