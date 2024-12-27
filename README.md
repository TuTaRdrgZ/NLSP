# Norminette LSP (C++)

This project is a C++ implementation of the **Language Server Protocol (LSP)** for **Norminette**, the linter used in the 42 school program for C code verification.

## Description

The goal of this project is to create an LSP server in C++ that interacts with code editors like **Neovim** or **VSC*de** to provide diagnostics and other features that improve the C development experience, following the LSP standard and using **Norminette** as the linting tool.

## Features

- **Diagnostics**: Uses Norminette to analyze C code and return warning and error messages.
- **LSP Compatibility**: Implements basic LSP methods such as `initialize`, `textDocument/didOpen`, `textDocument/didChange`, etc.
- **Editor Compatibility**: Can be integrated with editors that support LSP, such as **Neovim**, and others.

## Requirements

- **C++17** or higher
- **Norminette** installed on your system to run the code analysis
- **nlohmann/json** for JSON serialization and deserialization in C++

## Installation

### Cloning the repository and Building

```bash
git clone https://github.com/yourusername/norminette-lsp-cpp.git
cd norminette-lsp-cpp
make
```

### Neovim Integration

You can easily connect the server with the default neovim lsp support

```lua
local client = vim.lsp.start_client({
	name = "NLSP",
	cmd = { "path/to/NLSP/binary" },
	on_attach = function()
        -- set your keymaps/capabilities here
	end,
})

if not client then
	vim.notify("Failed to start NLSP")
	return
end

vim.api.nvim_create_autocmd("FileType", {
	pattern = { "h", "c" },

	callback = function()
        vim.g.c_syntax_for_h = 1 -- if you want to use the linter in .h files you need this line. see :h c.vim
		vim.lsp.buf_attach_client(0, client)
	end,
})
```
## Disclaimer

This project is a work in progress and is not yet finished. I created it as a learning exercise to understand how an LSP (Language Server Protocol) works, and using **Norminette** for this purpose seemed like a good choice.

Please note the following:

- There is a **logger** implemented, but you might want to remove it for your use case.
- The code is not polished and was developed over just a few hours.
- Many parts still need improvement and refinement.
- I may continue to work on it in the future, but there's no guarantee when or if that will happen.

Feel free to use or contribute to the project, but keep in mind that it may be incomplete or in an unstable state.
