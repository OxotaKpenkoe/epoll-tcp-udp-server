# Epoll TCP/UDP Server

Асинхронный сервер на C++17 для обработки TCP и UDP подключений с использованием epoll мультиплексирования.

## Возможности

- Асинхронная обработка клиентов через epoll
- Поддержка TCP и UDP протоколов
- Обработка команд:
  - `/time` - возврат текущего времени
  - `/stats` - статистика подключений
  - `/shutdown` - остановка сервера
- Эхо-режим для обычных сообщений

## Требования

- Ubuntu 24.04 или совместимый дистрибутив
- g++ с поддержкой C++17
- make
- systemd (для установки как сервис)

## Сборка
```bash
make
```

## Запуск
```bash
# Запуск с параметрами по умолчанию (TCP: 38080, UDP: 38081)
make run

# Или напрямую
./build/simple-server <tcp_port> <udp_port>
```

## Примеры использования
```bash
# TCP:
echo "/stats" | nc 127.0.0.1 38080

# UDP:
echo "/stats" | nc -u 127.0.0.1 38081
```

## Установка в систему

### Через .deb пакет:
```bash
# Создать пакет
make deb

# Установить
sudo dpkg -i simple-server_1.0.0_amd64.deb

# Запустить сервис
sudo systemctl start simple-server
sudo systemctl status simple-server

# Включить автозапуск
sudo systemctl enable simple-server

# Удалить
sudo dpkg -r simple-server
```

### Вручную:
```bash
# Скопировать бинарник
sudo cp build/simple-server /usr/local/bin/

# Скопировать systemd unit
sudo cp simple-server.service /etc/systemd/system/

# Запустить
sudo systemctl daemon-reload
sudo systemctl start simple-server
sudo systemctl enable simple-server
```
