




#let script(title: "", authors: (), body) = {
  // Set the document's basic properties.
  set document(author: authors, title: title)
  set page(numbering: "1", number-align: center, margin: (left: 3cm))
  set text(font: "Linux Libertine", lang: "de")

  // Heading behaviour
  set heading(numbering: "1.1.1",)

  // list indenting
  set list(body-indent: 1em)

  // Title row.
  v(1fr)
  block(text(1.2em, "Wintersemester 2023/24"))
  // block(text(1.2em, "BTU - Cottbus Senftenberg"))
  block(text(weight: 700, 2.5em, title))

  v(2.5em)

  block(text(1.2em, strong(
    for author in authors [#author #h(1em)]
  )))

  v(10em)
  
  pagebreak()

  // Main body.
  set par(justify: true, hanging-indent: 0pt,)

  body
}
