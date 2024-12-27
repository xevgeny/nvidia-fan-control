# Running nvidia-fan-control as a systemd service

1. Install the program:

```
sudo make install
```

By default, the program will be installed to `/usr/local/bin` and the config file will be installed to `/etc/nvidia-fan-control`.

2. Create a new file at /etc/systemd/system/nvidia-fan-control.service:

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

3. Enable and start the service:

```
sudo systemctl daemon-reload
sudo systemctl enable nvidia-fan-control.service
sudo systemctl start nvidia-fan-control.service
``` 

4. Check service status:

```
sudo systemctl status nvidia-fan-control.service
```