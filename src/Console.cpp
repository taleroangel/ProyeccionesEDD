#include "Console.h"

#include <iostream>
#include <sstream>

// Definiciones de la clase Command

Command::Command(std::string command,
                 caller_t caller,
                 std::string usage,
                 std::string description)
    : command(command), caller(std::move(caller)),
      usage(command + " " + usage), description(description)
{
}

std::string Command::get_command() const
{
    return this->command;
}

std::string Command::get_usage() const
{
    return this->usage;
}

std::string Command::get_description() const
{
    return this->description;
}

void Command::call(arguments_t args) const
{
    this->caller(args);
}

std::string Command::to_string() const
{
    return "Command: " + this->command + "\n" +
           "Caller type: " + typeid(this->caller).name() + "\n" +
           "Usage: " + this->usage + ".\n" +
           "Description: " + this->description + "\n";
}

// Definiciones de la excepción
Command::Error::Error(Command::Error::Type type, std::string what) : type(type), _what(what)
{
}

const char *Command::Error::what() const noexcept
{
    switch (this->type)
    {
    case INVALID_ARGS:
        return "Argumentos inválidos\n";
        break;

    case DOES_NOT_EXIST:
        return "El comando no existe\n";
        break;

    case FILE_ERROR:
        return "Error en la lectura de archivos";
        break;

    case BAD_USE:
        return _what.c_str();
        break;

    default:
        return "Comando inválido\n";
        break;
    }
}

const Command::Error::Type Command::Error::get_type() const
{
    return this->type;
}

// Definición de la clase Interpreter

Interpreter::Interpreter(std::vector<Command> commands) : commands(commands)
{
    Interpreter *this_interpreter = this;

    // Agregar información del comando ayuda
    this->commands.push_back(
        Command(
            _COMMAND_HELP, [=](Command::arguments_t args)
            { this_interpreter->show_help(args); },
            "[comando a consultar]",
            "Muestra información de ayuda sobre un comando"));

    // Agregar información del comando salir
    this->commands.push_back(
        Command(
            _COMMAND_EXIT, [=](Command::arguments_t args)
            { this_interpreter->_exit_ = true; },
            "", "Salir de la línea de comandos"));
}

void Interpreter::add_command(Command command)
{
    this->commands.push_back(command);
}

void Interpreter::cli()
{
    while (!_exit_)
    {
        // Mostrar el indicador
        std::cout << Interpreter::indicator << " " << std::flush;

        // Leer el comando
        std::string input;
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        // Tokenizar el comando
        std::stringstream str_stream(input);
        std::vector<std::string> tokens;
        std::string temp_str;

        while (std::getline(str_stream, temp_str, ' '))
            tokens.push_back(temp_str);

        // Buscar el comando y ejecutarlo
        bool _found_ = false;
        for (Command command : this->commands)
            if (command.get_command() == tokens[0])
            {
                _found_ = true;
                tokens.erase(tokens.begin());
                //! Capturar cualquier excepción que eleve
                try
                {
                    command.call(tokens);
                }
                catch (Command::Error &e)
                {
                    std::cerr << "(mensaje de error) " << e.what();
                    if (e.get_type() == Command::Error::Type::INVALID_ARGS)
                        std::cerr << "\tUso: " << command.get_usage() << std::endl;
                }
                break;
            }

        if (!_found_)
            std::cerr << "(mensaje de error) "
                      << "Comando inválido" << std::endl;
    }
}

void Interpreter::show_help(Command::arguments_t args) const
{
    // Mostrar todos los comandos línea por línea
    if (args.empty())
    {
        std::cout << "\nMostrando todos los comandos...\n";
        std::cout << "Uso: comando argumento1 [argumento2 opcional]\n";
        for (Command command : this->commands)
            std::cout
                << "\n> " << command.get_command()
                << ": " << command.get_description() << ".\n\tUso: "
                << command.get_usage() << "\n";
    }

    // Si se hace mal uso del comando
    else if (args.size() > 1)
        throw Command::Error(Command::Error::INVALID_ARGS);

    // Mostrar información de un comando
    else
    {
        // Buscar el comando especificado
        bool found = false;
        for (Command command : this->commands)
            if (command.get_command() == args[0])
            {
                std::cout
                    << "> " << command.get_command()
                    << ": " << command.get_description() << ".\n\tUso: "
                    << command.get_usage() << '\n';
                found = true;
                break;
            }

        // Si no se encontró el comando
        if (!found)
            throw Command::Error(Command::Error::DOES_NOT_EXIST);
    }

    std::cout << std::endl;
}
