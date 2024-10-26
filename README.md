# politech-labs-microprocessors

Лабораторные работы по предмету "Микропроцессорные системы" (универ, 5-й семестр)

## Лабораторная работа 1

1. Запустить виртуальную машину Zynq7000:

    ```bash
    qemu-system-arm -machine xilinx-zynq-a9 -m 128M -nographic -s -S -monitor stdio -serial tcp::5678,server,nowait
    ```

2. Подключиться по `socat`:

    ```bash
    socat TCP-CONNECT:127.0.0.1:5678 STDIO,icanon=0
    ```

3. Собрать и запустить проект;
4. Далее перейти в терминал с `socat` - там должно отобразиться:

    ```plaintext
    Hello, World!
    ```

## Лабораторная работа 2

1. Запустить виртуальную машину Zynq7000:

    ```bash
    qemu-system-arm -machine xilinx-zynq-a9 -m 128M -nographic -s -S -monitor stdio -serial tcp::5678,server,nowait
    ```

2. Поставить точки останова в коде;
3. Собрать и запустить проект;
4. Идти по точкам останова и смотреть на изменения в значениях регистров.

## Лабораторная работа 3

1. Запустить виртуальную машину Zynq7000:

    ```bash
    qemu-system-arm -machine xilinx-zynq-a9 -m 128M -nographic -s -S -monitor stdio -serial tcp::5678,server,nowait
    ```

2. Подключиться по `socat`:

    ```bash
    socat TCP-CONNECT:127.0.0.1:5678 STDIO,icanon=1
    ```

3. Собрать и запустить проект;
4. Далее перейти в терминал с `socat` и взаимодействовать с программой там:

    ```plaintext
    ========================================
    Enter first number: 16
    Enter second number: 21
    Enter operation ('+' or '-') or 'e' to exit: -
    Result: -5
    ========================================
    Enter first number: 13
    Enter second number: 9
    Enter operation ('+' or '-') or 'e' to exit: +
    Result: 22
    ========================================
    Enter first number: 10
    Enter second number: 11
    Enter operation ('+' or '-') or 'e' to exit: e
    ========================================
    Exiting...
    ```
