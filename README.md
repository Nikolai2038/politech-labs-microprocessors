# politech-labs-microprocessors

Лабораторные работы по предмету "Микропроцессорные системы" (универ, 5-й семестр)

## Лабораторная работа 1

Запустить виртуальную машину Zynq7000:

```bash
qemu-system-arm -machine xilinx-zynq-a9 -m 128M -nographic -s -S -monitor stdio -serial tcp::5678,server,nowait
```

Подключиться по `socat`:

```bash
socat TCP-CONNECT:127.0.0.1:5678 STDIO,icanon=0
```

## Лабораторная работа 2

Запустить виртуальную машину Zynq7000:

```bash
qemu-system-arm -machine xilinx-zynq-a9 -m 128M -nographic -s -S -monitor stdio -serial tcp::5678,server,nowait
```

## Лабораторная работа 3

Запустить виртуальную машину Zynq7000:

```bash
qemu-system-arm -machine xilinx-zynq-a9 -m 128M -nographic -s -S -monitor stdio -serial tcp::5678,server,nowait
```

Подключиться по `socat`:

```bash
socat TCP-CONNECT:127.0.0.1:5678 STDIO,icanon=1
```
