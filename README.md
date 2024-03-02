# typstfmt Ext

A Chromium extension that calls `typstfmt` on the current [Typst App](https://typst.app/) Document!

> [!CAUTION]
> This extension is in early development and may break your document. Backup your work before using it.

## Installation

> [!NOTE]
> I have submitted the extension
> to [Edge Add-ons](https://microsoftedge.microsoft.com/addons/Microsoft-Edge-Extensions-Home), but it is still under
> review. For now, you can install it manually.

1. Clone this repository.
2. Turn on "Developer Mode" in `edge://extensions/`.
3. Click "Load Unpacked" and select the `typstfmt-ext` directory.
4. The extension should now be installed.

To use the extension, the `typstfmt` executable must be installed on your system. You can install
it [here](https://github.com/astrale-sharp/typstfmt).

After installing the extension, you have to install
a [Native Messaging Host](https://learn.microsoft.com/en-us/microsoft-edge/extensions-chromium/developer-guide/native-messaging)
to communicate with the `typstfmt` executable. For Windows users, I've already compiled the host and included it in
the `host` directory. For other platforms, you can compile the host yourself with `cmake` and the
included `CMakeLists.txt`

Next, register the host. You can either follow the
instructions [here](https://learn.microsoft.com/en-us/microsoft-edge/extensions-chromium/developer-guide/native-messaging#step-3-register-the-native-messaging-host) (
refer to the following JSON)
or run the `host/register.ps1` file to do so.

```json
{
  "path": "<PATH_TO_HOST_EXECUTABLE>",
  "description": "TypeScript Formatter Extension Host",
  "name": "dev.flandia.typstfmt_ext_host",
  "allowed_origins": [
    "chrome-extension://ebkbgnmkkebikgiohnipickpecmcilep/"
  ],
  "type": "stdio"
}
```

Finally, if you registered the host by running `host/register.ps1`, open the generated `manifest.json` file and replace
the `"chrome-extension://ebkbgnmkkebikgiohnipickpecmcilep/"` with the ID of the extension on your machine.

> [!NOTE]
> Since the extension is not yet published, the ID of the extension is not determined. The `allowed_origins` field is
> set to the ID of the extension on my machine. The ID of the extension on your machine will be different. You can find
> it in `edge://extensions/` after installing it. 
