#include "ChronosObsidian.h"

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR"><head>
<meta charset="UTF-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Relogio Inteligente</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{font-family:'Courier New',monospace;background:#0a0a0f;color:#e0e0e0;padding:12px;max-width:700px;margin:0 auto}
h1{color:#38bdf8;font-size:1.3rem;margin-bottom:10px;text-align:center}
h2{color:#94a3b8;font-size:1rem;margin:14px 0 6px;border-bottom:1px solid #1a1a2e;padding-bottom:4px}
.clock{text-align:center;font-size:4rem;font-weight:bold;color:#4ade80;background:#111118;border:1px solid #1a1a2e;border-radius:12px;padding:16px 0 10px;margin:10px 0;letter-spacing:4px;text-shadow:0 0 20px #4ade8055}
.clock-sub{text-align:center;color:#64748b;font-size:0.85rem;margin-bottom:8px}
.card{background:#111118;border:1px solid #1a1a2e;border-radius:8px;padding:10px;margin:6px 0}
input[type=number],input[type=text],input[type=password]{width:110px;background:#1a1a2e;border:1px solid #333;color:#4ade80;padding:4px;border-radius:4px;text-align:center;font-family:inherit}
select{background:#1a1a2e;border:1px solid #333;color:#4ade80;padding:4px;border-radius:4px;font-family:inherit}
button{background:#1e40af;color:#fff;border:none;padding:5px 12px;border-radius:5px;cursor:pointer;margin:3px;font-size:0.85rem}
button:hover{background:#2563eb}.on{background:#16a34a}.on:hover{background:#15803d}.off{background:#dc2626}.off:hover{background:#b91c1c}
.row{display:flex;align-items:center;gap:6px;flex-wrap:wrap;margin:3px 0}.lbl{color:#64748b;min-width:180px;font-size:0.85rem}.sep{color:#555}
.tabs{display:flex;gap:4px;margin:10px 0}.tab{flex:1;padding:8px;text-align:center;background:#111118;border:1px solid #1a1a2e;border-radius:6px 6px 0 0;cursor:pointer;color:#64748b;font-size:0.78rem}
.tab.active{background:#1a1a2e;color:#4ade80;border-bottom:2px solid #4ade80}
.page{display:none}.page.active{display:block}
.tpanel{display:none;background:#0a0a0f;border:1px solid #1a1a2e;border-radius:0 0 6px 6px;padding:10px;min-height:200px;max-height:320px;overflow-y:auto;font-size:0.78rem;line-height:1.5;white-space:pre-wrap;color:#4ade80}
.tpanel.active{display:block}.tpanel.cgraf{white-space:normal;max-height:none;padding:8px}
.cgrid{display:grid;grid-template-columns:1fr 1fr;gap:12px;margin-top:10px}.ccard{background:#111118;border:1px solid #1a1a2e;border-radius:12px;padding:12px;overflow:visible}
.sbtn{display:block;width:100%;padding:9px;background:#1d4ed8;color:#fff;border:none;border-radius:7px;cursor:pointer;font-size:0.9rem;font-family:inherit}
</style></head><body>
<h1>Relogio Inteligente</h1>
<div class="tabs"><div class="tab active" onclick="sp(0)">Principal</div><div class="tab" onclick="sp(1)">Monitoramento</div><div class="tab" onclick="sp(2)">Changelog</div><div class="tab" onclick="sp(3)">Sobre</div></div>
<div id="pg0" class="page active">
<div class="clock" id="clk">--:--:--</div>
<div class="clock-sub"><span id="tmp">--</span> | <span id="upt">--</span></div>
<h2>Hora do Sistema</h2><div class="card"><div class="row"><input type="number" id="sh" min="0" max="23" value="0"><span class="sep">:</span><input type="number" id="sm" min="0" max="59" value="0"><button onclick="setH()">Salvar Hora</button></div></div>
<h2>Alarmes</h2><div id="als"></div>
<h2>Clima</h2><div class="card"><div class="row"><span id="tmpLocal" class="lbl">Temperatura Local (DHT): --</span><span id="humLocal" class="lbl">Umidade Local (DHT): --</span><span id="tmpApi" class="lbl">Previsao do Tempo (API): --</span><button onclick="doClima()">Atualizar Clima</button></div></div>
</div>
<div id="pg1" class="page">
<h2>Monitoramento do Sistema</h2>
<div class="tabs"><div class="tab active" onclick="stab(0)">Logs</div><div class="tab" onclick="stab(1)">Serial</div><div class="tab" onclick="stab(2)">Flash</div><div class="tab" onclick="stab(3)">Graficos</div></div>
<div class="tpanel active" id="tp0">carregando...</div>
<div class="tpanel" id="tp1">carregando serial...</div>
<div class="tpanel" id="tp2">carregando flash...</div>
<div class="tpanel cgraf" id="tp3"><button class="sbtn" id="btnSalvar" onclick="forcaSalvar()">Forcar Salvamento na Flash</button><button class="sbtn" id="btnDownload" style="background:#065f46" onclick="baixarBackup()">Baixar Backup TXT</button><input type="file" id="arquivoBackup" accept=".txt" style="display:none" onchange="restaurarBackup(this)"><button class="sbtn" style="background:#7c3aed" onclick="document.getElementById('arquivoBackup').click()">Restaurar Backup TXT</button><div id="perfStatus" style="margin:8px 0;padding:10px;background:#111118;border:1px solid #1a1a2e;border-radius:8px;font-size:0.85rem;color:#64748b;">carregando...</div><div class="cgrid"><div class="ccard"><canvas id="pc0"></canvas></div><div class="ccard"><canvas id="pc1"></canvas></div><div class="ccard"><canvas id="pc2"></canvas></div><div class="ccard"><canvas id="pc3"></canvas></div><div class="ccard"><canvas id="pc4"></canvas></div><div class="ccard"><canvas id="pc5"></canvas></div><div class="ccard"><canvas id="pc6"></canvas></div></div></div>
</div>
<div id="pg2" class="page">
<h2>Changelog do Sistema</h2>
<div class="tpanel active" id="tpChangelog" style="max-height:none;min-height:400px;">carregando changelog...</div>
</div>
<div id="pg3" class="page">
<h2>Sobre e Configuracoes</h2>
<div class="card"><b>Sobre o Projeto Chronos Obsidian</b><br>Universidade: Pontifícia Universidade Católica do Paraná (PUCPR).<br>Integrantes: EMANUEL HENRIQUE MARCINEK SILVA, GABRIEL MOLETA ROSSI, JULIO MIGUEL CZAPLINSKI KUCHUMINSKI, MURILO BIZZ, MURILO PRIZIBELA.<br>GitHub: <a href="https://github.com/emanuelhmarcineksilva/Relogio" style="color:#4ade80">Link Repositório</a><br>MAC Address: <span id="sysMac" style="color:#38bdf8">--</span><br>Uptime: <span id="sysUpt" style="color:#38bdf8">--</span></div>
<div class="card"><b>Guia de Changelog (V4.1.0)</b><br>• Implementação de Light Sleep com despertar por GPIO (botões) e temporizador.<br>• Persistência de estado via RTC_DATA_ATTR para métricas críticas.<br>• Sistema de Watchdog integrado para prevenção de travamentos.<br>• Changelog persistente gerenciado via SPIFFS e endpoint REST.<br>• Otimização de interrupções de hardware com debounce via timer de alta precisão.</div>
<div class="card"><b>Configuracoes</b><br>Use estes campos para ajustar leitura dos sensores, intervalo do clima e backup da flash sem recompilar o firmware.<div class="row"><span class="lbl">Leitura DHT (ms)</span><input id="cfgDht" type="number" min="2000"></div><div class="row"><span class="lbl">Clima API (ms)</span><input id="cfgClima" type="number" min="60000"></div><div class="row"><span class="lbl">Backup Flash (ms)</span><input id="cfgBackup" type="number" min="300000"></div><div class="row"><span class="lbl">Cidade (Meteosource)</span><input id="cfgCidade" type="text"></div><button onclick="salvarCfg()">Salvar Configuracoes</button></div>
<div class="card"><b>Troca de WiFi</b><div class="row"><span class="lbl">SSID</span><input id="wssid" type="text"></div><div class="row"><span class="lbl">Senha</span><input id="wpass" type="password"></div><button onclick="salvarWifi()">Salvar WiFi</button><div id="wifiResp"></div></div>
</div>
<script>
const M=['Aurora','Sino','Despertar','Fur Elise','Estrela'];
const PN=['Loop','Clima API','RTC','Botoes','Display','Leitura DHT','Logica Alarme'];
const PC=['#4ade80','#38bdf8','#f97316','#a78bfa','#facc15','#f472b6','#22d3ee'];
function sp(n){document.querySelectorAll('.page').forEach((p,i)=>p.classList.toggle('active',i==n));document.querySelectorAll('body>.tabs .tab').forEach((t,i)=>t.classList.toggle('active',i==n));if(n==1)gS();if(n==2)gCg();if(n==3)gCfg();}
function stab(n){document.querySelectorAll('#pg1 .tab').forEach((t,i)=>t.classList.toggle('active',i==n));document.querySelectorAll('#pg1 .tpanel').forEach((p,i)=>p.classList.toggle('active',i==n));if(n==2)gF();if(n==3)gPerf();}
function fixCanvas(c){const w=c.parentElement.clientWidth-24||240;c.width=w;c.height=140;c.style.width=w+'px';c.style.height='140px';}
function drawChart(id,data,color,name,curUs){
    const c=document.getElementById(id);if(!c)return;fixCanvas(c);const ctx=c.getContext('2d'),w=c.width,h=c.height;
    const mgL=55,mgB=25,mgT=20,mgR=15,cH=h-mgB-mgT,cW=w-mgL-mgR;
    ctx.clearRect(0,0,w,h);ctx.fillStyle='#0d0d16';ctx.fillRect(mgL,mgT,cW,cH);
    ctx.strokeStyle='#1e2038';ctx.lineWidth=1;
    for(let i=0;i<4;i++){const y=mgT+cH*i/3;ctx.beginPath();ctx.moveTo(mgL,y);ctx.lineTo(mgL+cW,y);ctx.stroke();}
    
    // Convert us to ms for readability if it's a performance metric
    const toMs = v => (v / 1000).toFixed(2);
    const label = "ms";
    const cur = toMs(curUs);

    if(!data||data.length<2){
        ctx.fillStyle=color;ctx.font='bold 11px monospace';ctx.textAlign='left';ctx.fillText(name+': '+cur+' '+label,mgL,mgT-8);
        ctx.fillStyle='#64748b';ctx.font='10px monospace';ctx.textAlign='center';ctx.fillText('Aguardando amostras...',mgL+cW/2,mgT+cH/2);return;
    }
    
    const realData=data.map(v=>v/1000); // converting full set to ms
    const mx=Math.max(...realData,parseFloat(cur),0.1),mn=0;
    const padding=mx*0.1,yMax=mx+padding,yMin=0,rng=yMax-yMin||1;
    const px=i=>mgL+cW*i/(data.length-1),py=v=>mgT+cH-(v-yMin)/rng*cH;
    
    const grad=ctx.createLinearGradient(0,mgT,0,mgT+cH);grad.addColorStop(0,color+'44');grad.addColorStop(1,color+'05');
    ctx.fillStyle=grad;ctx.beginPath();ctx.moveTo(px(0),mgT+cH);realData.forEach((v,i)=>ctx.lineTo(px(i),py(v)));ctx.lineTo(px(data.length-1),mgT+cH);ctx.closePath();ctx.fill();
    
    ctx.strokeStyle=color;ctx.lineWidth=2;ctx.shadowColor=color;ctx.shadowBlur=8;ctx.beginPath();realData.forEach((v,i)=>i?ctx.lineTo(px(i),py(v)):ctx.moveTo(px(i),py(v)));ctx.stroke();ctx.shadowBlur=0;
    
    ctx.strokeStyle='#334155';ctx.lineWidth=1.5;ctx.beginPath();ctx.moveTo(mgL,mgT);ctx.lineTo(mgL,mgT+cH);ctx.lineTo(mgL+cW,mgT+cH);ctx.stroke();
    
    ctx.fillStyle=color;ctx.font='bold 12px monospace';ctx.textAlign='left';ctx.fillText(`${name}: ${cur}${label}`,mgL,mgT-8);
    
    ctx.fillStyle='#94a3b8';ctx.font='9px monospace';ctx.textAlign='right';
    for(let i=0;i<=3;i++){
        const yFrac=i/3,yVal= (yMax - (rng*yFrac)).toFixed(1);
        const yPx=mgT+cH*yFrac;ctx.fillText(yVal,mgL-6,yPx+3);
    }
    
    ctx.textAlign='center';ctx.font='8px monospace';
    // X-axis: 0 on the right, -Total on the left
    const total=data.length;
    for(let i=0;i<=4;i++){
        const sampleIdx = Math.round((total-1) * i/4);
        const labelIdx = -(total - 1 - sampleIdx);
        const xPx = px(sampleIdx);
        ctx.fillText(labelIdx==0?'Agora':labelIdx, xPx, mgT+cH+15);
    }
    ctx.fillText('Amostras', mgL+cW/2, mgT+cH+25);
}
async function gPerf(){try{const r=await fetch('/api/perf');const d=await r.json();const pos=d.pos,full=d.full,total=full?288:pos,pct=Math.round(total/288*100);
const el=document.getElementById('perfStatus');if(el)el.innerHTML=`<span style="color:#4ade80">${total}</span>/288 pontos &bull; Mem: <span style="color:#38bdf8">${(d.heap/1024).toFixed(1)}KB</span> &bull; CPU: <span style="color:#f97316">${d.cpu?d.cpu.toFixed(1):0}%</span>`;
requestAnimationFrame(()=>{for(let f=0;f<7;f++){const raw=d.hist[f];let ord=full?[...raw.slice(pos),...raw.slice(0,pos)]:raw.slice(0,pos);
drawChart('pc'+f,ord,PC[f],PN[f],d.atual[f]);}});}catch(e){console.error('[gPerf]',e);}}
async function forcaSalvar(){await fetch('/api/salvar');}
async function baixarBackup(){const r=await fetch('/api/download_perf');const txt=await r.text();const blob=new Blob([txt],{type:'text/plain'});const url=URL.createObjectURL(blob);const a=document.createElement('a');a.href=url;a.download='backup_perf.txt';a.click();URL.revokeObjectURL(url);}
async function restaurarBackup(input){if(!input.files.length)return;const txt=await input.files[0].text();const r=await fetch('/api/upload_perf',{method:'POST',headers:{'Content-Type':'text/plain'},body:txt});alert(await r.text());input.value='';}
function rA(d){let h='';for(let i=0;i<3;i++){const a=d.alarmes[i];h+='<div class="card"><div class="row"><span class="lbl">Alm '+(i+1)+'</span><input type="number" id="ah'+i+'" min="0" max="23" value="'+a.h+'"><span class="sep">:</span><input type="number" id="am'+i+'" min="0" max="59" value="'+a.m+'"><select id="ml'+i+'">';for(let j=0;j<M.length;j++)h+='<option value="'+j+'"'+(j==a.mel?' selected':'')+'>'+M[j]+'</option>';h+='</select><button class="'+(a.a?'on':'off')+'" onclick="tg('+i+')">'+(a.a?'ON':'OFF')+'</button><button onclick="sv('+i+')">Salvar</button></div></div>';}document.getElementById('als').innerHTML=h;document.getElementById('tmp').textContent='Temperatura Local (DHT): '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');document.getElementById('tmpLocal').textContent='Temperatura Local (DHT): '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');document.getElementById('humLocal').textContent='Umidade Local (DHT): '+(d.dhtHum>-100?d.dhtHum.toFixed(1)+'%':'--');document.getElementById('tmpApi').textContent='Previsao do Tempo (API): '+d.temp.toFixed(1)+'°C';}
async function gS(){try{const r=await fetch('/api/log');document.getElementById('tp0').textContent=await r.text()}catch(e){}}
async function gSer(){try{const r=await fetch('/api/serial');const el=document.getElementById('tp1');el.textContent=await r.text();el.scrollTop=el.scrollHeight}catch(e){}}
async function gF(){try{const r=await fetch('/api/flash');document.getElementById('tp2').textContent=await r.text()}catch(e){}}
async function gCg(){try{const r=await fetch('/api/changelog');const txt=await r.text();document.getElementById('tpChangelog').textContent=txt;}catch(e){console.error('[gCg]',e);}}
let editandoAlarmes=false;
document.addEventListener('focusin',e=>{if(e.target.closest&&e.target.closest('#als'))editandoAlarmes=true;});
document.addEventListener('focusout',e=>{if(e.target.closest&&e.target.closest('#als')){setTimeout(()=>{if(!document.querySelector('#als input:focus,#als select:focus'))editandoAlarmes=false;},200);}});
async function gA(){try{const r=await fetch('/api/alarmes');const d=await r.json();if(!editandoAlarmes)rA(d);document.getElementById('clk').textContent=d.hora;document.getElementById('upt').textContent='Up: '+d.uptime+'s';document.getElementById('tmp').textContent='Temperatura Local (DHT): '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');document.getElementById('tmpLocal').textContent='Temperatura Local (DHT): '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');document.getElementById('humLocal').textContent='Umidade Local (DHT): '+(d.dhtHum>-100?d.dhtHum.toFixed(1)+'%':'--');document.getElementById('tmpApi').textContent='Previsao do Tempo (API): '+d.temp.toFixed(1)+'°C';}catch(e){}}
async function gCfg(){try{const r=await fetch('/api/config');const d=await r.json();document.getElementById('cfgDht').value=d.dhtMs;document.getElementById('cfgClima').value=d.climaMs;document.getElementById('cfgBackup').value=d.backupMs;document.getElementById('cfgCidade').value=d.cidade||'curitiba';document.getElementById('wssid').value=d.ssid||'';document.getElementById('sysMac').textContent=d.mac||'--';const u=document.getElementById('upt').textContent;document.getElementById('sysUpt').textContent=u.replace('Up: ','');}catch(e){}}async function salvarCfg(){const d=document.getElementById('cfgDht').value,c=document.getElementById('cfgClima').value,b=document.getElementById('cfgBackup').value,cid=encodeURIComponent(document.getElementById('cfgCidade').value);await fetch('/api/config/set?dht='+d+'&clima='+c+'&backup='+b+'&cidade='+cid);}

async function salvarWifi(){const s=encodeURIComponent(document.getElementById('wssid').value),p=encodeURIComponent(document.getElementById('wpass').value);const r=await fetch('/api/wifi/set?ssid='+s+'&pass='+p);document.getElementById('wifiResp').textContent=await r.text();}
async function setH(){const h=document.getElementById('sh').value,m=document.getElementById('sm').value;await fetch('/api/hora?h='+h+'&m='+m);}
async function sv(i){const h=document.getElementById('ah'+i).value,m=document.getElementById('am'+i).value,ml=document.getElementById('ml'+i).value;await fetch('/api/alarme?id='+i+'&h='+h+'&m='+m+'&mel='+ml);editandoAlarmes=false;gA();}
async function tg(i){await fetch('/api/alarme/toggle?id='+i);gA();}
async function doClima(){await fetch('/api/clima');gA();}
gA();gS();gSer();setInterval(gA,2000);setInterval(gS,3000);setInterval(gSer,2000);setInterval(()=>{if(document.getElementById('tp3').classList.contains('active'))gPerf();},5000);
</script></body></html>
)rawliteral";
