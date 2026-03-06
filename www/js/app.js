document.addEventListener('DOMContentLoaded', () => {
    // Elements
    const rpsEl = document.getElementById('rps-value');
    const totalEl = document.getElementById('total-requests');
    const activeEl = document.getElementById('active-connections');
    const bytesEl = document.getElementById('bytes-sent');
    
    const sendBtn = document.getElementById('send-btn');
    const endpointSel = document.getElementById('endpoint-select');
    const respBody = document.getElementById('response-body');
    const respStatus = document.getElementById('response-status');
    const respTime = document.getElementById('response-time');
    
    const benchBtn = document.getElementById('benchmark-btn');
    const benchResults = document.getElementById('benchmark-results');
    const benchProgress = document.getElementById('benchmark-progress');
    const benchAvg = document.getElementById('bench-avg');
    const benchMin = document.getElementById('bench-min');
    const benchMax = document.getElementById('bench-max');
    const benchTotal = document.getElementById('bench-total');

    // State
    let lastRequests = 0;
    let lastTime = Date.now();

    // 1. Poll Metrics
    async function fetchMetrics() {
        try {
            const res = await fetch('/api/metrics');
            if (!res.ok) throw new Error('Failed to fetch metrics');
            const data = await res.json();
            
            // Calculate RPS
            const now = Date.now();
            const timeDiff = (now - lastTime) / 1000; // seconds
            const reqDiff = data.total_requests - lastRequests;
            
            if (timeDiff > 0 && lastRequests > 0) {
                const rps = (reqDiff / timeDiff).toFixed(1);
                rpsEl.textContent = rps;
            }
            
            // Update other metrics
            totalEl.textContent = data.total_requests.toLocaleString();
            activeEl.textContent = data.active_connections;
            bytesEl.textContent = (data.bytes_sent / 1024 / 1024).toFixed(2) + ' MB';
            
            lastRequests = data.total_requests;
            lastTime = now;
        } catch (err) {
            console.error(err);
        }
    }

    // Initial fetch and interval
    fetchMetrics();
    setInterval(fetchMetrics, 1000);

    // 2. API Tester
    sendBtn.addEventListener('click', async () => {
        const url = endpointSel.value;
        const start = performance.now();
        
        respStatus.textContent = 'Status: Sending...';
        respBody.textContent = 'Loading...';
        
        try {
            const res = await fetch(url);
            const end = performance.now();
            const latency = (end - start).toFixed(2);
            
            respStatus.textContent = `Status: ${res.status} ${res.statusText}`;
            respTime.textContent = `Latency: ${latency} ms`;
            
            const contentType = res.headers.get('content-type');
            if (contentType && contentType.includes('application/json')) {
                const json = await res.json();
                respBody.textContent = JSON.stringify(json, null, 2);
            } else {
                const text = await res.text();
                respBody.textContent = text.substring(0, 1000) + (text.length > 1000 ? '...' : '');
            }
        } catch (err) {
            respStatus.textContent = 'Status: Error';
            respBody.textContent = err.toString();
        }
    });

    // 3. Mini Benchmark
    benchBtn.addEventListener('click', async () => {
        const COUNT = 100;
        const URL = '/api/hello'; // Benchmark against a simple JSON endpoint
        
        benchBtn.disabled = true;
        benchResults.style.display = 'block';
        benchProgress.style.width = '0%';
        
        let latencies = [];
        const totalStart = performance.now();
        
        for (let i = 0; i < COUNT; i++) {
            const start = performance.now();
            try {
                await fetch(URL);
            } catch (e) {
                console.error(e);
            }
            const end = performance.now();
            latencies.push(end - start);
            
            // Update progress
            const pct = ((i + 1) / COUNT) * 100;
            benchProgress.style.width = `${pct}%`;
        }
        
        const totalEnd = performance.now();
        
        // Stats
        const sum = latencies.reduce((a, b) => a + b, 0);
        const avg = (sum / latencies.length).toFixed(2);
        const min = Math.min(...latencies).toFixed(2);
        const max = Math.max(...latencies).toFixed(2);
        const total = (totalEnd - totalStart).toFixed(0);
        
        benchAvg.textContent = `${avg} ms`;
        benchMin.textContent = `${min} ms`;
        benchMax.textContent = `${max} ms`;
        benchTotal.textContent = `${total} ms`;
        
        benchBtn.disabled = false;
    });
});
