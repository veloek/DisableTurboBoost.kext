# DisableTurboBoost

A MacOS kernel extension (kext) for disabling CPU turbo boost to keep temperature (and fan noise) down.

## Usage

Since System Integrity Protection (SIP) blocks unsigned kernel extensions, there are a few steps involved in enabling this kext:

1. Reboot Mac into recovery mode by holding down cmd+R when booting
2. While in recovery mode, open terminal and execute `csrutil enable --without kext`
3. Reboot back into normal mode
4. Open a terminal in the folder containing the kext and execute:
```
sudo chown -R root:wheel DisableTurboBoost.kext
sudo chmod -R 0755 DisableTurboBoost.kext
sudo kextload DisableTurboBoost.kext
```

Run `kextstat |grep DisableTurboBoost` to verify that it's loaded.

## How it works

This small kernel extension will enable bit 32 of the MSR register `MSR_IA32_PERF_CTL`. This causes the CPU to disable turbo boost, while keeping speed step.

Other similar extensions choose to enable bit 38 of `MSR_IA32_MISC_ENABLE`, but this also disables speed step causing the CPU to run at a higher frequency (and drain battery faster) than necessary.

Take a look in the [manual](http://www.intel.com/content/www/us/en/processors/architectures-software-developer-manuals.html) for more info.

## Disclamer

I take no responsibility for whatever harm this kernel extension may cause your system. I use it myself without any issues, but I cannot guarantee it will work for you.

## Troubleshooting

If you get this message when executing `sudo kextload DisableTurboBoost.kext`:
```
failed to load - (libkern/kext) not found; check the system/kernel logs for errors or try kextutil(8)
```
Try this command in stead: `sudo kextutil -l DisableTurboBoost.kext`

It will output something about invalid signatures, but the extension might still be loaded. Verify with `kextstat |grep DisableTurboBoost`.

## Credits

The code was written by Vegard Løkken with great inspiration from [nanoant/DisableTurboBoost.kext](https://github.com/nanoant/DisableTurboBoost.kext) and [pyrovski/powertools](https://github.com/pyrovski/powertools).
