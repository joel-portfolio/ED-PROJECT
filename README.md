# Library Management System

A comprehensive library management system developed in C for managing books, requesters (patrons), and book requests. The system uses hash tables for efficient book organization and linked lists for managing requesters and requests.

## 📋 Features

### Book Management

- Add, view, and manage books in the library
- Organize books by subject areas using hash tables
- Track book availability and request status
- ISBN-based book identification
- View most recent books and most requested books
- Identify areas with the most books

### Requester (Patron) Management

- Register and manage library patrons
- Store personal information including ID, name, and birth date
- Geographic location tracking (district, county, parish)
- Age calculation and demographics analysis
- Search by name, surname, ID, or location
- Track oldest requesters and average age

### Request Management

- Request books from the library
- Track request and delivery dates
- Manage active and completed requests
- Search requests by requester ID or ISBN
- View all requested books with detailed information

### Data Management

- Persistent storage using text files
- XML export functionality
- Memory usage monitoring and reporting
- Automatic data saving on exit

## 🏗️ Project Structure

### Core Components

```
ED_PROJECT/
├── MAIN.C                 # Main program entry and menu system
├── BOOK.C/H              # Book management functionality
├── REQUESTS.C/H          # Request management system
├── requester.c/h         # Requester (patron) management
├── HASHING.C/H           # Hash table implementation for books
├── LIST.C/H              # Generic linked list implementation
├── geo_entities.c/h      # Geographic entities (districts, counties, parishes)
├── db_management.c/h     # Database file operations
├── memo_management.c/h   # Memory management utilities
├── USEFUL.C/H            # Utility functions
└── XML_R/                # XML export directory
```

### Data Files

```
├── books.txt             # Book records
├── requesters.txt        # Requester/patron records
├── requests.txt          # Book request records
├── distritos.txt         # District data
├── concelhos.txt         # County data
├── freguesias.txt        # Parish data
└── logs.txt              # System logs
```

## 🚀 Getting Started

### Prerequisites

- Microsoft Visual Studio (2017 or later)
- Windows SDK 10.0
- C Compiler with C11 support

### Building the Project

#### Using Visual Studio

1. Open `ED_PROJECT.sln` in Visual Studio
2. Select your build configuration (Debug/Release)
3. Build the solution (Ctrl+Shift+B)

#### Using MSBuild

```cmd
msbuild ED_PROJECT.sln /t:build /p:Configuration=Release
```

#### Using the included task

Run the build task configured in the workspace.

### Running the Application

After building, execute the binary from the Debug or Release folder:

```cmd
cd Debug
ED_PROJECT.exe
```

## 📖 Usage

### Main Menu Options

1. **Manage Books** - Add, view, and manage library books
2. **Manage Requesters** - Register and manage library patrons
3. **Manage Book Requests** - Handle book borrowing and returns
4. **Memory Usage** - View current memory allocation
5. **Export to XML** - Save data in XML format
6. **Save Data** - Save all data to text files
7. **Exit** - Save and exit the application

### Data Format

The system automatically loads data from text files on startup and saves changes on exit. Manual saves can be performed through the menu.

## 🗂️ Data Structures

### Hash Table (Library)

Books are organized by subject area using a hash table implementation with chaining for collision resolution.

### Generic Linked Lists

Used for:

- Districts, counties, and parishes
- Requesters (patrons)
- Book requests
- Books within each subject area

## 🔧 Technical Details

### Memory Management

- Dynamic memory allocation tracking
- Memory usage reporting in bytes, KB, and MB
- Proper cleanup on program exit

### Geographic Hierarchy

- **Districts** (Distritos) - Top-level administrative divisions
- **Counties** (Concelhos) - Mid-level divisions within districts
- **Parishes** (Freguesias) - Local-level divisions within counties

### Timestamps

The system tracks dates for:

- Requester birth dates
- Book request dates
- Book delivery dates

## 📝 Language

The interface is in Portuguese, designed for Portuguese-speaking library systems. Key terms:

- **Requisitante** - Requester/Patron
- **Requisição** - Request/Loan
- **Livro** - Book
- **Distrito** - District
- **Concelho** - County
- **Freguesia** - Parish

## 🤝 Contributing

This appears to be an academic project (ED_PROJECT likely stands for "Estruturas de Dados" - Data Structures).

## 📄 License

Academic project - check with the repository owner for usage rights.

## 👥 Authors

Check the repository owner: joel-portfolio

## 🔍 Additional Notes

- The project uses Windows-specific configurations
- Portuguese locale is set for proper character encoding
- All file paths use forward slashes for cross-platform compatibility where possible
- The system includes validation for geographic locations and requester information
