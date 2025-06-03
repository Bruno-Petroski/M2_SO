# Trabalho para M2 de Sistemas Operacionais 

### Alunos
- Bruno Petroski Enghi
- Gabriel Bortolocci
  
---

## Casos de Teste

### 16 BITS

#### 256B
Endereço = 1.000
- Página: 3
- Deslocamento: 232
- Ação tomada: TLB Hit
- Valor lido: 44

Endereço = 10.000
- Página: 39
- Deslocamento: 16
- Ação tomada: TLB Miss, Page Hit
- Valor lido: 58

Endereço = 30.000
- Página: 117
- Deslocamento: 48
- Ação tomada: TLB Miss, Page Fault, carregado da backing store
- Valor lido: 13

Endereço = 50.000
- Página: 195
- Deslocamento: 80
- Ação tomada: TLB Miss, Page Fault, não encontrado na backing store
- Valor lido: -
  
#### 1KB
Endereço = 500
- Página: 0
- Deslocamento: 500
- Ação tomada: TLB Hit
- Valor lido: 55

Endereço = 20.000
- Página: 19
- Deslocamento: 544
- Ação tomada: TLB Miss, Page Hit
- Valor lido: 28

Endereço = 50.000
- Página: 48
- Deslocamento: 848
- Ação tomada: TLB Miss, Page Fault, carregado da backing store
- Valor lido: 78

Endereço = 55.000
- Página: 53
- Deslocamento: 728
- Ação tomada: TLB Miss, Page Fault, não encontrado na backing store
- Valor lido: -


#### 4KB
Endereço = 400
- Página: 0
- Deslocamento: 400
- Ação tomada: TLB Hit
- Valor lido: 5

Endereço = 58.000
- Página: 14
- Deslocamento: 656
- Ação tomada: TLB Miss, Page Hit
- Valor lido: 32

