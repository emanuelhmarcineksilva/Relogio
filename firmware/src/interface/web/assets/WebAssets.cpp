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
<h2>Hora do Sistema</h2><div class="card" id="cardSys"><div class="row"><input type="number" id="sh" min="0" max="23" value="0"><span class="sep">:</span><input type="number" id="sm" min="0" max="59" value="0"><button onclick="setH()">Salvar Hora</button></div></div>
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
<div class="card"><b>Guia de Changelog (V4.1.0)</b><br>• <b>[ENERGIA]</b> Sistema de Idle Timeout (50s) com controle de Light Sleep - timer resetável por cliques, previne sleep com atividade.<br>• <b>[RTOS]</b> Arquitetura Dual-Core com 4 tarefas independentes (Sensores, UI, Audio, Persistência).<br>• <b>[OBSERVABILIDADE]</b> Dashboard Web, Log Ring Buffer (8KB), Histórico de Performance com gráficos em tempo real.<br>• <b>[SENSORES]</b> RTC DS3231, DHT22, WiFi + Meteosource API para previsão de tempo.<br>• <b>[AUDIO]</b> Síntese I2S com 5 melodias para alarmes com reprodução em alta fidelidade.<br>• <b>[PERSISTENCIA]</b> Backup CRC32 de configurações, logs e métricas em SPIFFS com restauração automática.</div>
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
    const toMs = v => (v / 1000).toFixed(2);
    const cur = toMs(curUs);
    if(!data||data.length<2){
        ctx.fillStyle=color;ctx.font='bold 11px monospace';ctx.textAlign='left';ctx.fillText(name+': '+cur+' ms',mgL,mgT-8);
        ctx.fillStyle='#64748b';ctx.font='10px monospace';ctx.textAlign='center';ctx.fillText('Aguardando amostras...',mgL+cW/2,mgT+cH/2);return;
    }
    const realData=data.map(v=>v/1000);
    const mx=Math.max(...realData,parseFloat(cur),0.1);
    const padding=mx*0.1,yMax=mx+padding,yMin=0,rng=yMax-yMin||1;
    const px=i=>mgL+cW*i/(data.length-1),py=v=>mgT+cH-(v-yMin)/rng*cH;
    const grad=ctx.createLinearGradient(0,mgT,0,mgT+cH);grad.addColorStop(0,color+'44');grad.addColorStop(1,color+'05');
    ctx.fillStyle=grad;ctx.beginPath();ctx.moveTo(px(0),mgT+cH);realData.forEach((v,i)=>ctx.lineTo(px(i),py(v)));ctx.lineTo(px(data.length-1),mgT+cH);ctx.closePath();ctx.fill();
    ctx.strokeStyle=color;ctx.lineWidth=2;ctx.shadowColor=color;ctx.shadowBlur=8;ctx.beginPath();realData.forEach((v,i)=>i?ctx.lineTo(px(i),py(v)):ctx.moveTo(px(i),py(v)));ctx.stroke();ctx.shadowBlur=0;
    ctx.strokeStyle='#334155';ctx.lineWidth=1.5;ctx.beginPath();ctx.moveTo(mgL,mgT);ctx.lineTo(mgL,mgT+cH);ctx.lineTo(mgL+cW,mgT+cH);ctx.stroke();
    ctx.fillStyle=color;ctx.font='bold 12px monospace';ctx.textAlign='left';ctx.fillText(`${name}: ${cur}ms`,mgL,mgT-8);
    ctx.fillStyle='#94a3b8';ctx.font='9px monospace';ctx.textAlign='right';
    for(let i=0;i<=3;i++){const yVal=(yMax-rng*i/3).toFixed(1);ctx.fillText(yVal,mgL-6,mgT+cH*i/3+3);}
}
async function gPerf(){try{const r=await fetch('/api/perf');const d=await r.json();const pos=d.pos,full=d.full,total=full?288:pos;
const el=document.getElementById('perfStatus');if(el)el.innerHTML=`<span style="color:#4ade80">${total}</span>/288 pontos &bull; Mem: <span style="color:#38bdf8">${(d.heap/1024).toFixed(1)}KB</span> &bull; CPU: <span style="color:#f97316">${d.cpu?d.cpu.toFixed(1):0}%</span>`;
requestAnimationFrame(()=>{for(let f=0;f<7;f++){const raw=d.hist[f];let ord=full?[...raw.slice(pos),...raw.slice(0,pos)]:raw.slice(0,pos);
drawChart('pc'+f,ord,PC[f],PN[f],d.atual[f]);}});}catch(e){console.error('[gPerf]',e);}}
async function forcaSalvar(){await fetch('/api/salvar');}
async function baixarBackup(){const r=await fetch('/api/download_perf');const txt=await r.text();const blob=new Blob([txt],{type:'text/plain'});const url=URL.createObjectURL(blob);const a=document.createElement('a');a.href=url;a.download='backup_perf.txt';a.click();URL.revokeObjectURL(url);}
async function restaurarBackup(input){if(!input.files.length)return;const txt=await input.files[0].text();const r=await fetch('/api/upload_perf',{method:'POST',headers:{'Content-Type':'text/plain'},body:txt});alert(await r.text());input.value='';}

function rA(d){
    const div=document.getElementById('als');
    if(!div.innerHTML || div.innerHTML.indexOf('ah0') === -1) {
        let h='';
        for(let i=0;i<3;i++){
            const a=d.alarmes[i];
            h+='<div class="card"><div class="row">';
            h+='<span class="lbl">Alm '+(i+1)+'</span>';
            h+='<input type="number" id="ah'+i+'" min="0" max="23" value="'+a.h+'" oninput="this.dataset.dirty=1">';
            h+='<span class="sep">:</span>';
            h+='<input type="number" id="am'+i+'" min="0" max="59" value="'+a.m+'" oninput="this.dataset.dirty=1">';
            h+='<select id="ml'+i+'" oninput="this.dataset.dirty=1">';
            for(let j=0;j<M.length;j++) h+='<option value="'+j+'"'+(j==a.mel?' selected':'')+'>'+M[j]+'</option>';
            h+='</select>';
            h+='<button id="tg'+i+'" onclick="tg('+i+')">--</button>';
            h+='<button onclick="sv('+i+')">Salvar</button>';
            h+='</div></div>';
        }
        div.innerHTML=h;
    }
    
    // Verifica se ALGUÉM na seção de alarmes está sendo editado ou tem o foco
    const isEditing = div.contains(document.activeElement);

    for(let i=0;i<3;i++){
        const a=d.alarmes[i];
        const hI=document.getElementById('ah'+i), mI=document.getElementById('am'+i), sL=document.getElementById('ml'+i), bT=document.getElementById('tg'+i);
        
        // SÓ atualiza os campos se o usuário não estiver mexendo na seção E o campo não estiver "sujo"
        if(!isEditing && hI.dataset.dirty !== "1") hI.value = a.h;
        if(!isEditing && mI.dataset.dirty !== "1") mI.value = a.m;
        if(!isEditing && sL.dataset.dirty !== "1") sL.value = a.mel;
        
        bT.className = a.a ? 'on' : 'off';
        bT.textContent = a.a ? 'ON' : 'OFF';
    }
    document.getElementById('tmp').textContent='DHT: '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');
    document.getElementById('tmpLocal').textContent='Temp Local: '+(d.dhtTemp>-100?d.dhtTemp.toFixed(1)+'°C':'--');
    document.getElementById('humLocal').textContent='Hum Local: '+(d.dhtHum>-100?d.dhtHum.toFixed(1)+'%':'--');
    document.getElementById('tmpApi').textContent='Previsao API: '+d.temp.toFixed(1)+'°C';
}

async function gS(){try{const r=await fetch('/api/log');document.getElementById('tp0').textContent=await r.text()}catch(e){}}
async function gSer(){try{const r=await fetch('/api/serial');const el=document.getElementById('tp1');el.textContent=await r.text();el.scrollTop=el.scrollHeight}catch(e){}}
async function gF(){try{const r=await fetch('/api/flash');document.getElementById('tp2').textContent=await r.text()}catch(e){}}
async function gCg(){try{const r=await fetch('/api/changelog');document.getElementById('tpChangelog').textContent=await r.text();}catch(e){}}

let sk=null;
function uC(){if(!sk)return;sk.setSeconds(sk.getSeconds()+1);document.getElementById('clk').textContent=sk.toTimeString().split(' ')[0];}
async function gA(){
    try{
        const r=await fetch('/api/alarmes');const d=await r.json();
        rA(d);
        const p=d.hora.split(':');
        sk=new Date();sk.setHours(p[0]);sk.setMinutes(p[1]);sk.setSeconds(p[2]);
        document.getElementById('clk').textContent=d.hora;
        document.getElementById('upt').textContent='Up: '+d.uptime+'s';
        
        const cardSys=document.getElementById('cardSys');
        const isEditingSys = cardSys.contains(document.activeElement);
        const sh=document.getElementById('sh'), sm=document.getElementById('sm');
        
        if(!isEditingSys && (sh.value == "0" && sm.value == "0")){
            sh.value=parseInt(p[0]); sm.value=parseInt(p[1]);
        }
    }catch(e){}
}

async function gCfg(){try{const r=await fetch('/api/config');const d=await r.json();document.getElementById('cfgDht').value=d.dhtMs;document.getElementById('cfgClima').value=d.climaMs;document.getElementById('cfgBackup').value=d.backupMs;document.getElementById('cfgCidade').value=d.cidade||'curitiba';document.getElementById('wssid').value=d.ssid||'';document.getElementById('sysMac').textContent=d.mac||'--';const u=document.getElementById('upt').textContent;document.getElementById('sysUpt').textContent=u.replace('Up: ','');}catch(e){}}
async function salvarCfg(){const d=document.getElementById('cfgDht').value,c=document.getElementById('cfgClima').value,b=document.getElementById('cfgBackup').value,cid=encodeURIComponent(document.getElementById('cfgCidade').value);try{const r=await fetch('/api/config/set?dht='+d+'&clima='+c+'&backup='+b+'&cidade='+cid);if(r.ok){alert('✓ OK');await gCfg();}}catch(e){alert('✗ Erro');}}
async function salvarWifi(){const s=encodeURIComponent(document.getElementById('wssid').value),p=encodeURIComponent(document.getElementById('wpass').value);try{const r=await fetch('/api/wifi/set?ssid='+s+'&pass='+p);if(r.ok)document.getElementById('wifiResp').textContent='✓ '+await r.text();}catch(e){}}
async function setH(){const h=document.getElementById('sh').value,m=document.getElementById('sm').value;await fetch('/api/hora?h='+h+'&m='+m);await gA();}
async function sv(i){
    const hI=document.getElementById('ah'+i), mI=document.getElementById('am'+i), sL=document.getElementById('ml'+i);
    const h=hI.value, m=mI.value, ml=sL.value;
    await fetch('/api/alarme?id='+i+'&h='+h+'&m='+m+'&mel='+ml);
    hI.dataset.dirty="0"; mI.dataset.dirty="0"; sL.dataset.dirty="0";
    await gA();
}
async function tg(i){await fetch('/api/alarme/toggle?id='+i);await gA();}
async function doClima(){await fetch('/api/clima');gA();}

gA();gS();gSer();
setInterval(uC,1000);
setInterval(gA,10000); 
setInterval(gS,10000);
setInterval(gSer,10000);
setInterval(()=>{if(document.getElementById('tp3').classList.contains('active'))gPerf();},30000);
</script></body></html>
)rawliteral";
