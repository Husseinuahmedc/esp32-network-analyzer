let socket;
let rssiChart;

function setStatus(connected) {
  const badge = document.getElementById("ws-status");
  if (!badge) return;

  if (connected) {
    badge.textContent = "● Connected";
    badge.classList.remove("badge-bad");
    badge.classList.add("badge-ok");
  } else {
    badge.textContent = "● Disconnected";
    badge.classList.remove("badge-ok");
    badge.classList.add("badge-bad");
  }
}

function connectWebSocket() {
  socket = new WebSocket(`ws://${window.location.hostname}/ws`);

  socket.onopen = () => {
    setStatus(true);
  };

  socket.onclose = () => {
    setStatus(false);
    setTimeout(connectWebSocket, 2000);
  };

  socket.onmessage = (event) => {
    try {
      const data = JSON.parse(event.data);
      if (!Array.isArray(data)) return;
      updateUI(data);
    } catch (e) {
      console.error("Invalid JSON from ESP32:", e);
    }
  };
}

function updateUI(networks) {
  updateTable(networks);
  updateChart(networks);
}

function updateTable(networks) {
  const tbody = document.getElementById("networks-body");
  if (!tbody) return;

  tbody.innerHTML = "";

  networks.forEach((n) => {
    const ssid = n.ssid && n.ssid.length ? n.ssid : "(hidden)";
    const rssi = n.rssi;
    const channel = n.channel ?? n.ch ?? "-";
    const auth = n.auth ?? "-";

    const tr = document.createElement("tr");

    const ssidTd = document.createElement("td");
    ssidTd.textContent = ssid;
    tr.appendChild(ssidTd);

    const rssiTd = document.createElement("td");
    rssiTd.textContent = rssi;
    rssiTd.className = rssiClass(rssi);
    tr.appendChild(rssiTd);

    const chTd = document.createElement("td");
    chTd.textContent = channel;
    tr.appendChild(chTd);

    const authTd = document.createElement("td");
    authTd.textContent = auth;
    tr.appendChild(authTd);

    tbody.appendChild(tr);
  });
}

function updateChart(networks) {
  const canvas = document.getElementById("rssiChart");
  if (!canvas) return;

  const sorted = [...networks]
    .filter((n) => typeof n.rssi === "number")
    .sort((a, b) => b.rssi - a.rssi)
    .slice(0, 10);

  const labels = sorted.map((n) =>
    n.ssid && n.ssid.length ? n.ssid : "(hidden)"
  );
  const data = sorted.map((n) => n.rssi);

  if (!rssiChart) {
    const ctx = canvas.getContext("2d");
    rssiChart = new Chart(ctx, {
      type: "bar",
      data: {
        labels,
        datasets: [
          {
            label: "RSSI (dBm)",
            data,
          },
        ],
      },
      options: {
        responsive: true,
        scales: {
          y: {
            beginAtZero: false,
          },
        },
      },
    });
  } else {
    rssiChart.data.labels = labels;
    rssiChart.data.datasets[0].data = data;
    rssiChart.update();
  }
}

function rssiClass(rssi) {
  if (rssi >= -50) return "rssi-good";
  if (rssi >= -70) return "rssi-medium";
  return "rssi-bad";
}

document.addEventListener("DOMContentLoaded", connectWebSocket);

