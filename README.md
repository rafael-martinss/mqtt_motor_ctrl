# Como validar
Com base nos exercicios em aula, foi mantido o destino de broker bem como conexões de hardware

## Controle PWM
Para o controle de PWM, foi adotado que o corpo da mensagem apresentará o valor em porcentagem (0 à 100), para controle de conversão devida de forma interna.

Para se efetuar o controle do valor, a mensagem de protoclo deverá seguir o seguinte modelo:
```text
duty_cycle:<PERCENT_NUMBER>>
```
>**OBS:** Sem espaço entre ':' e o valor numérico!

## Indicativo sentido de rotação
Como o motor não possui explicitamente uma forma fácil de se ter o que é *sentido horário* e *sentido anti-horário*, foi atodado o conceito de sentido **normal** e **reverso**. A troca será feita pelo botão nativo do HW da RPI, em release ele será estado "normal" e pressionado "reverso".

- Solicitação:
```text
get_rotation
```

- Resposta:
```text
Motor current rotation: normal(or reverse) 
```

# Configurando os dados da rede Wi-fi
As informações devem ser alteradas no [wifi_config.h](wifi_config.h). Caso se esqueca, será gerado alguns erros de compilação intensionais!!