# ANÁLISE COMPARATIVA: relogio3.ino vs relogio6.ino
## Funções Relacionadas a Gráficos

---

## 1. FUNÇÃO `drawChart` (JavaScript)

**Localização:**
- relogio3.ino: Linha 633
- relogio6.ino: Linha 1101

**Status:** IDÊNTICA em ambas as versões

**Código (formatado para legibilidade):**
```javascript
function drawChart(id,data,color,name,cur,label='us'){
  const c=document.getElementById(id);
  if(!c)return;
  fixCanvas(c);
  const ctx=c.getContext('2d'),w=c.width,h=c.height;
  ctx.fillStyle='#0d0d16';
  ctx.fillRect(0,0,w,h);
  ctx.strokeStyle='#1e2038';
  for(let i=0;i<4;i++){
    const y=10+(h-20)*i/3;
    ctx.beginPath();
    ctx.moveTo(0,y);
    ctx.lineTo(w,y);
    ctx.stroke();
  }
  if(!data||data.length<2){
    ctx.fillStyle=color;
    ctx.font='bold 10px monospace';
    ctx.fillText(name+': '+cur+label,4,14);
    return;
  }
  const mx=Math.max(...data,1),mn=Math.min(...data,0),
        sp=Math.max(mx-mn,1),
        yMax=mx+sp*0.3,yMin=Math.max(mn-sp*0.3,0),
        rng=yMax-yMin||1,
        px=i=>w*i/(data.length-1),
        py=v=>h-10-(v-yMin)/rng*(h-20);
  const grad=ctx.createLinearGradient(0,0,0,h);
  grad.addColorStop(0,color+'44');
  grad.addColorStop(1,color+'08');
  ctx.fillStyle=grad;
  ctx.beginPath();
  data.forEach((v,i)=>i?ctx.lineTo(px(i),py(v)):ctx.moveTo(px(i),py(v)));
  ctx.lineTo(w,h-10);
  ctx.lineTo(0,h-10);
  ctx.closePath();
  ctx.fill();
  ctx.strokeStyle=color;
  ctx.lineWidth=2;
  ctx.shadowColor=color;
  ctx.shadowBlur=10;
  ctx.beginPath();
  data.forEach((v,i)=>i?ctx.lineTo(px(i),py(v)):ctx.moveTo(px(i),py(v)));
  ctx.stroke();
  ctx.shadowBlur=0;
  ctx.fillStyle=color;
  ctx.font='bold 10px monospace';
  ctx.fillText(name+': '+cur+label,4,14);
  ctx.fillStyle='#64748b';
  ctx.font='9px monospace';
  ctx.fillText('max:'+mx+label+' min:'+mn+label,4,26);
}
```

---

## 2. FUNÇÃO `gPerf` (JavaScript)

**Localização:**
- relogio3.ino: Linha 634
- relogio6.ino: Linha 1102

**Status:** IDÊNTICA em ambas as versões

**Código (formatado para legibilidade):**
```javascript
async function gPerf(){
  try{
    const r=await fetch('/api/perf');
    const d=await r.json();
    for(let f=0;f<7;f++){
      const raw=d.hist[f],
            pos=d.pos,
            full=d.full,
            ord=full?[...raw.slice(pos),...raw.slice(0,pos)]:raw.slice(0,pos);
      const label=(f===4)?'°C':(f===6?'%':'us');
      const current=(f===4||f===6)?(d.atual[f]/10).toFixed(1):d.atual[f];
      drawChart('pc'+f,ord,PC[f],PN[f],current,label);
    }
  }catch(e){}
}
```

**O que faz:**
1. Busca dados de `/api/perf` (JSON)
2. Para cada uma das 7 funções monitoradas (f=0 a 6):
   - Reordena o array de histórico (ring buffer)
   - Define o label correto (°C para [4], % para [6], us para os outros)
   - Converte valores de temp/umidade (x10 para valor real)
   - Chama `drawChart()` para desenhar o gráfico

---

## 3. ROTA `/api/perf` (Backend C++)

**Localização:**
- relogio3.ino: Linha 1130-1182
- relogio6.ino: Linha 1797-1849

**Status:** IDÊNTICA em ambas as versões

**Estrutura do JSON retornado:**
```json
{
  "atual": [
    tempoLoopUs,
    tempoPegarClimaUs,
    tempoRtcNowUs,
    tempoLeituraBotoesUs,
    dhtTemperature*10,
    tempoAtualizarDisplayUs,
    dhtHumidity*10
  ],
  "hist": [
    [valor1, valor2, ...],  // função 0 (Loop)
    [valor1, valor2, ...],  // função 1 (Clima)
    [valor1, valor2, ...],  // função 2 (RTC)
    [valor1, valor2, ...],  // função 3 (Botões)
    [valor1, valor2, ...],  // função 4 (DHT Temp x10)
    [valor1, valor2, ...],  // função 5 (Display)
    [valor1, valor2, ...]   // função 6 (DHT Umidade x10)
  ],
  "pos": perfHistPos,       // posição atual no ring buffer
  "full": bool,             // buffer já deu uma volta completa?
  "cpu": cpuUsoEstimado,
  "heap": memóriaLivre,
  "minHeap": memóriaMinima,
  "stack": bytesStack,
  "flashUsed": bytesUsados,
  "flashTotal": bytesTotal,
  "psramTotal": memPSRAM,
  "psramFree": memPSRAMlivre,
  "tasks": quantidadeTarefas,
  "wifi": "CONECTADO|DESCONECTADO",
  "ap": "ATIVO|INATIVO"
}
```

---

## 4. PRINCIPAIS DIFERENÇAS

### DIFERENÇA 1: Tamanho do Buffer Histórico

| Aspecto | relogio3.ino | relogio6.ino |
|---------|-------------|-------------|
| **PERF_HIST_SIZE** | 60 | 288 |
| **PERF_SAMPLE_MS** | 5000 (5 seg) | 300000 (5 min) |
| **Período de retenção** | 5 min (60×5seg) | 24 horas (288×5min) |
| **Memória por função** | 240 bytes | 1152 bytes |
| **Total (7 funções)** | ~1.7 KB | ~8 KB |

### DIFERENÇA 2: Validação de Integridade de Dados

**relogio3.ino:**
- Sem proteção CRC
- `salvarPerfFlash()`: Escreve apenas perfHist, perfHistPos, perfHistFull
- `carregarPerfFlash()`: Valida apenas o tamanho esperado do arquivo

**relogio6.ino:**
- Possui validação CRC32 (FASE 2.4)
- Implementa função `crc32_simple()` para calcular CRC
- `salvarPerfFlash()`: 
  - Calcula CRC32 antes de salvar
  - Escreve: perfHist + perfHistPos + perfHistFull + CRC
- `carregarPerfFlash()`: 
  - Lê o CRC armazenado
  - Recalcula o CRC e compara
  - Se inválido, reinicia o buffer com zeros
  - Log detalhado de validação

**Tamanho do arquivo de histórico:**
- relogio3.ino: 60×7×4 + 4 + 1 = 1685 bytes
- relogio6.ino: 288×7×4 + 4 + 1 + 4 = 8093 bytes

### DIFERENÇA 3: Documentação de Comentários

**relogio3.ino:**
- Menciona 6 funções inicialmente
- Depois atualiza para 7 funções (comentário confuso)
- Menciona requisito de 24h mas define apenas 60 amostras

**relogio6.ino:**
- FASE 2.4 bem documentada
- Explicação clara de por que 288 amostras
- Cálculo de memória explícito (~8KB)
- Documentação de CRC clara

---

## 5. ESTRUTURA DO HTML (Canvas Elements)

**Localização:** Ambas têm estrutura idêntica

```html
<div class="cgrid">
  <div class="ccard"><canvas id="pc0" height="90"></canvas></div>
  <div class="ccard"><canvas id="pc1" height="90"></canvas></div>
  <div class="ccard"><canvas id="pc2" height="90"></canvas></div>
  <div class="ccard"><canvas id="pc3" height="90"></canvas></div>
  <div class="ccard"><canvas id="pc4" height="90"></canvas></div>
  <div class="ccard"><canvas id="pc5" height="90"></canvas></div>
  <div class="ccard"><canvas id="pc6" height="90"></canvas></div>
</div>
```

**Sempre 7 gráficos** (pc0 até pc6), correspondendo às 7 funções monitoradas.

---

## 6. RÓTULOS DOS GRÁFICOS

**Localização (ambas linhas equivalentes):**
```javascript
const PN=['Loop','Clima API','RTC','Botoes','DHT Temp','Display','DHT Umidade'];
const PC=['#4ade80','#38bdf8','#f97316','#a78bfa','#facc15','#f472b6','#22d3ee'];
```

**Mapeamento:**
- pc0: Loop (verde)
- pc1: Clima API (azul)
- pc2: RTC (laranja)
- pc3: Botões (roxo)
- pc4: DHT Temp (amarelo)
- pc5: Display (rosa)
- pc6: DHT Umidade (ciano)

---

## 7. PROCESSA DADOS NA FUNÇÃO `gPerf()`

**Processamento de dados:**

```javascript
const raw = d.hist[f];              // array cru do backend
const pos = d.pos;                  // posição atual do ring buffer
const full = d.full;                // buffer completo?
// Reordena para ordem cronológica
const ord = full ? 
  [...raw.slice(pos), ...raw.slice(0,pos)] : 
  raw.slice(0, pos);

// Labels específicos
const label = (f===4) ? '°C' : (f===6 ? '%' : 'us');

// Valor atual (divide por 10 se for temp/umidade)
const current = (f===4 || f===6) ? 
  (d.atual[f]/10).toFixed(1) : 
  d.atual[f];

// Desenha gráfico
drawChart('pc'+f, ord, PC[f], PN[f], current, label);
```

---

## 8. IDENTIFICAÇÃO DO PROBLEMA NO relogio6.ino

### Problema Potencial:

O relogio6.ino mudou para **PERF_SAMPLE_MS = 300000 (5 minutos)**, mas:

1. **No loop()** (seção de amostragem), o código que incrementa `perfHistPos` pode não estar atualizado
2. A validação CRC é mais rigorosa - se o formato antigo for carregado, será rejeitado
3. Se houver incompatibilidade no cálculo de CRC, os dados podem ser perdidos

### O que precisa ser verificado:

1. Procurar pela variável `ultimaAmostraPerf` e verificar onde é usada
2. Procurar pelo `perfHistPos = (perfHistPos + 1) % PERF_HIST_SIZE` para garantir que está usando 288 corretamente
3. Verificar se a amostragem está realmente acontecendo a cada 5 minutos (300000ms)

---

## 9. RESUMO DAS CORREÇÕES NECESSÁRIAS NO relogio6.ino

**NENHUMA correção necessária nas funções de gráficos!**

A lógica de `drawChart()`, `gPerf()`, e `/api/perf` estão corretas e idênticas entre as duas versões.

**Possíveis verificações no relogio6.ino:**

1. Confirmar que `ultimaAmostraPerf` é atualizado cada 5 minutos no `loop()`
2. Verificar que `perfHistPos` incrementa corretamente com PERF_HIST_SIZE=288
3. Validar que o CRC está sendo calculado/verificado corretamente
4. Confirmar que o intervalo de 5 minutos está sendo respeitado (não 5 segundos)

