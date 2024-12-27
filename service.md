# Running nvidia-fan-control as a systemd service

Create a new file at /etc/systemd/system/nvidia-fan-control.service:

```
[Unit]
Description=NVIDIA fan control service
After=nvidia-persistenced.service
Wants=nvidia-persistenced.service

[Service]
Type=simple
ExecStart=/usr/local/bin/nvidia-fan-control /etc/nvidia-fan-control/config.cfg
Restart=on-failure
RestartSec=3
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
```

Enable and start the service:

```
sudo systemctl daemon-reload
sudo systemctl enable nvidia-fan-control.service
sudo systemctl start nvidia-fan-control.service
``` 

Check service status:

```
sudo systemctl status nvidia-fan-control.service
```

View logs:

```
journalctl -u nvidia-fan-control.service
```