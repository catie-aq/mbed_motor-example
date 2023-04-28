# mbed_motor-example

An example of using the 6TRON Motor library with MbedOS

## Requirements

### Hardware requirements

The following boards are required:

- *List mbed_motor-example hardware requirements here*

### Software requirements

mbed_motor-example makes use of the following libraries (automatically
imported by `mbed deploy` or `mbed import`):

- *List mbed_motor-example software requirements here*

## Usage

This repository use MbedOS as RTOS framework. While next steps describes how to use Mbed CLI 1 commands to setup the project, it is highly recommended that users read [MbedOS official documentation](https://os.mbed.com/docs/mbed-os/v6.16/build-tools/create.html).

To clone **and** deploy the project in one command, use `mbed import` and skip to the
target and toolchain definition:
```shell
mbed import https://github.com/catie-aq/mbed_motor-example.git mbed_motor-example
```

Alternatively:

- Clone to "mbed_motor-example" and enter it:

  ```shell
  git clone https://github.com/catie-aq/mbed_motor-example.git mbed_motor-example
  cd mbed_motor-example
  ```

- Deploy software requirements with:

  ```shell
  mbed deploy
  ```

- If necessary, eg using a 6TRON Zest Core, clone the custom target repository:

  ```shell
  git clone YOUR_CUSTOM_TARGET_REPOSITORY your-custom-target
  ```
* ... and enable the custom target:

  * Linux:

    ```shell
    cp your-custom-target/custom_targets.json .
    ```

  * Windows:

    ```shell
    copy your-custom-target\custom_targets.json .
    ```

Define your target and toolchain:

```shell
mbed target YOUR_MBED_OS_TARGET
mbed toolchain GCC_ARM
```

Compile the project:

```shell
mbed compile
```

Program the target device with a Segger J-Link debug probe and
[`sixtron_flash`](https://github.com/catie-aq/6tron-flash) tool:

```shell
sixtron_flash
```

Debug on the target device with the probe and Segger [Ozone](https://www.segger.com/products/development-tools/ozone-j-link-debugger) software.
