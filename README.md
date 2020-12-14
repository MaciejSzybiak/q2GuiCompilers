## q2GuiCompilers

This application is a graphical user interface version of the console tools for compiling Quake 2 map files. All the tools are included within the application which allows for effortless adjustments of the compilation process.

![app](docs/img/app.png)

### Getting started

After downloading a release and unzipping it the application is ready to use. First launch will create a configuration file and a cache profile in your local appdata folder.

Application's window is split into two panels. The left panel contains all the available settings and the right panel is where the console output appears.

##### Export running app as standalone

After launching the application expand the **Map** panel and select the map file you want to compile. In the **Game** panel you should select the folder in which your game is located (*Directory* option). All other settings are optional, so after adjusting them you can hit the **Compile** button and your map will be compiled.

##### Export using command line

The application can be launched using command line which is useful for various map editors. You can specify a few arguments when going for that option.
* `-profile <profile_name>` will load the selected profile when the application starts
* `-instant` will automatically trigger compilation as soon as the application is launched
* last argument is always the path to a map file. It can be relative to the working directory or a full path to any file in your filesystem

### Building

Clone this repository using `git clone --recurse-submodules https://github.com/MaciejSzybiak/q2GuiCompilers.git` command. Use Visual Studio 2019 to open included solution file and build the program.

### Notes

Creating this application wouldn't be possible without using the following code:
* [microui](https://github.com/rxi/microui)
* [Glad](https://glad.dav1d.de/)
* [glfw](https://github.com/glfw/glfw)
* [Quake 2 compiling utilities](http://home.insightbb.com/~gryndehl/q2compile/quake2.html) made by Geoffrey DeWan
* [blarghrad](https://github.com/zzsort/blarghrad)
