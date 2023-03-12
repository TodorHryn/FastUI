# FastUI

FastUI is a small C++ GUI library. 

The main goal of FastUI is to provide good-looking, fast UI and make GUI programming as simple and convenient as possible.

Features:
- Unicode support
- All rendering through OpenGL
- Same look on different operating systems

# Gallery

![Calculator](docs/calculator.png)
![All controls](docs/controls.png)

# Usage

See examples/ folder

### Creating button
```cpp
int main()
{
    auto button = make_shared<Button>();
    button->m_text = "Hello, world!";
	button->setOnMouseEvent([&](const MouseEvent& ev) {
		button->m_text = "Pressed!";
		return true;
	});

    auto layout = make_shared<LinearLayout>();
	layout->addChild(make_shared<Spacer>());
    layout->addChild(button);
	layout->addChild(make_shared<Spacer>());

	auto drawer = std::make_shared<DrawerOpenGL>(1280, 720);
	drawer->setRoot(layout);
	drawer->execute();
	return 0;
}
```

# Building

Requirements:
- C++11 or newer
- OpenGL 3.3 or newer

Dependencies:
- glfw3
- glad
- glm
- stb
- Freetype
- ICU

The easiest way to build is to install vcpkg, create environmental variable VCPKG_PATH (e.g., ```SET VCPKG_PATH="C:\vcpkg"```) and launch ```build-shared.bat```

# Credits

Embeds Open Sans, Noto Sans Symbols, Noto Emoji fonts

# License

FastUI is licensed under the MIT License, see [LICENSE.txt](https://github.com/TodorHryn/FastUI/blob/master/LICENSE.txt) for more information.
