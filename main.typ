#import "template.typ": *

#show: script.with(
  title: "Parser Project",
  authors: (
    "Timo Plieth",
    "Jakob Ziechmann",
  )
)

= Grammatik
```
Ausdruck        -> '(' Ausdruck ')' | InfixAusdruck | PrefixAusdruck 
InfixAusdruck   -> Ausdruck InfixOperator Ausdruck 
PrefixAusdruck  -> PrefixAusdruck Ausdruck
InfixOperator   -> '&' | '|' | '=>' | '<=' | '<=>'
PrefixAusdruck  -> '!'
```

