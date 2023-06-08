# peekaboo
Projeto da Disciplina MC833 - Programação de Redes de Computadores

O **peekaboo** é uma aplicação simples com o fim de gerenciar perfis profissionais e acadêmicos de pessoas. A aplicação funciona no modelo cliente e servidor, podendo operar nos protocolos TCP ou UDP. O nome **peekaboo** foi uma sugestão do ChatGPT com base nos requisitos do sistema e faz referência a brincadeira de esconde-esconde feita com crianças, fazendo alusão às informações dos perfis que a aplicação revela.

## Developers

- Cristiano Sampaio Pinheiro ([CristianoS16](https://github.com/CristianoS16))
- Jhonatan Cléto ([Cl3TO](https://github.com/Cl3TO))

## Build

Para fazer o `build` da aplicação basta utilizar o seguinte procedimento:

```bash
chmod +x scripts/build_peekaboo.sh
scripts/build_peekaboo.sh
```

Com isso, os binários do cliente (`client`) e do servidor (`server`) estarão disponíveis no diretório bin. Além disso, uma instancia de banco de dados com 20 perfis estara disponível em `bin/profiles.db`.

## Run

Devido a localização do arquivo com o banco de dados, para que o **peekaboo** funcione corretamente é necessário executar os binário dentro do diretório `bin`.

Primeiro execute o binário do servidor utilizando o comando:

```bash
cd bin
./server
```

Ainda no diretório `bin`, inicie o binário do cliente, indicando opcionalmente o ip do servidor (localhost por padrão) utilizando a flag `--server`.
```bash
./client --server <server_ip>
```

Por padrão o peekaboo opera no modo UDP, caso deseje utilizar o protocolo TCP inicie ambos servidor e cliente com a flag `--tcp`. Exemplo:
```bash
cd bin
./server --tcp &
./client --server <server_ip> --tcp
```

## Exemplo de funcionamento

No diretório de exemplos, deixamos um vídeo ilustrando o funcionamento do [**peekaboo**](examples/peekaboo_demo.mp4).

![**peekaboo**](examples/peekaboo_demo.gif)