#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class CuentaCliente
{
private:
    string NumeroCuenta;
    string Tipocuenta; // cc , ca
    int Saldo_cliente;

public:
    CuentaCliente() {}
    CuentaCliente(string numero, string tipo, int saldo) : NumeroCuenta(numero), Tipocuenta(tipo), Saldo_cliente(saldo) {}

    // getters y setters
    string getNumeroCuenta() const { return NumeroCuenta; }
    string getTipoCuenta() const { return Tipocuenta; }
    int getSaldoCliente() const { return Saldo_cliente; }

    void Depositar(int monto)
    {
        Saldo_cliente += monto;
    }
    bool Retirar(int cantidad)
    {
        if (cantidad <= Saldo_cliente)
        {
            Saldo_cliente -= cantidad;
            return true;
        }
        else
        {
            return false;
        }
    }

    void setNumeroCuenta(string numero) { NumeroCuenta = numero; }
    void setTipoCuenta(string tipo) { Tipocuenta = tipo; }
    void setSaldoCliente(int saldo) { Saldo_cliente = saldo; }
};
class Usuarios
{
private:
    string Nombre;
    string Apellido;
    int Cedula;
    CuentaCliente Cuenta;
    string Pass;
    bool Conectado;

public:
    Usuarios() {}

    Usuarios(string nombre, string apellido, int dni, CuentaCliente cuenta, string p) : Nombre(nombre), Apellido(apellido), Cedula(dni), Cuenta(cuenta), Pass(p), Conectado(false) {}

    // getters
    string getName() { return Nombre; }
    string getApellido() { return Apellido; }
    int getCedula() { return Cedula; }
    CuentaCliente &getCuentaCliente() { return Cuenta; }
    string getPass() { return Pass; }
    int getSaldo()
    {
        return Cuenta.getSaldoCliente();
    }
    bool EstaConectado() const { return Conectado; }
    // setters
    void setName(const string nombre) { Nombre = nombre; }
    void setApellido(const string apellido) { Apellido = apellido; }
    void setCedula(int dni) { Cedula = dni; }
    void setPassword(string p) { Pass = p; }
    // estado de usuario
    void Conectar() { Conectado = true; }
    void Desconectar() { Conectado = false; }
};

class Transaccion
{
private:
    string idTransaccion;
    int Monto;
    string Estado_Transaccion; // Fallido o Exitoso
    string TipoTrans;          // tipo transaccion: retiro o deposito

public:
    Transaccion(string id, int cantidad, string tipo) : idTransaccion(id), Monto(cantidad), TipoTrans(tipo) {}

    string getIdTransaccion() const { return idTransaccion; }
    int getMonto() const { return Monto; }
    string getTipoTransaccion() const { return TipoTrans; }
    string getEstado() const { return Estado_Transaccion; }

    void setEstadoFallido(string estado)
    {
        Estado_Transaccion = estado;
    }
    void setEstadoExitoso(string estado)
    {
        Estado_Transaccion = estado;
    }
};

class Banco
{
private:
    string NombreBanco;
    string Tipo;                            // internacional o nacional
    vector<Transaccion> ListaTransacciones; // lista de todas las transacciones que estan asociados al banco
    vector<Usuarios> ListaClientes;         // lista de los clientes asociados al banco
    Usuarios *usuarioConectado;             // puntero que apunta a los usuarios conectados
                                            // Listado de Bancos registrados
    vector<Banco> Lista_Bancos;

public:
    Banco() {} // constructor por defecto
    Banco(string nombre, string tipo) : NombreBanco(nombre), Tipo(tipo), usuarioConectado(nullptr) {}

    Usuarios *getClientePorCedula(int cedula)
    {
        for (Usuarios &user : ListaClientes)
        {
            //   cout << "Revisando usuario con cedula: " << user.getCedula() << endl;
            if (user.getCedula() == cedula)
            {
                //      cout << "Usuario encontrado." << endl;
                return &user;
            }
        }
        cout << "Usuario no encontrado." << endl;
        return nullptr;
    }
    Usuarios *getClientePorTipoNumero(string tipo, string numero)
    {
        for (Usuarios &user : ListaClientes)
        {
            if (user.getCuentaCliente().getNumeroCuenta() == numero && user.getCuentaCliente().getTipoCuenta() == tipo)
            {
                return &user;
            }
        }
        return nullptr; // si no encontro ningun cliente que cumpla con los requisitos devolvera null
    }
    string getNombreBanco() const
    {
        return NombreBanco;
    }
    string getTipo() const
    {
        return Tipo;
    }
    Banco *getBanco(string nombre) // retorna el puntero del banco cuyo nombre es nombre
    {
        for (Banco &banco : Lista_Bancos)
        {
            if (banco.getNombreBanco() == nombre)
            {
                return &banco;
            }
        }
        return nullptr;
    }
    bool ExisteNumeroCuenta(string numeroCuenta)
    {
        for (Usuarios &user : ListaClientes)
        {
            if (user.getCuentaCliente().getNumeroCuenta() == numeroCuenta)
            {
                return true;
            }
        }
        return false;
    }
    bool ExisteCedula(int cedula)
    {
        for (auto &user : ListaClientes)
        {
            if (user.getCedula() == cedula)
            {
                return true;
            }
        }
        return false;
    }
    // seterrs
    void AltaUsuario(const Usuarios &nuevoUsuario)
    { // agrego un nuevo usuario al banco
        ListaClientes.push_back(nuevoUsuario);
    }

    void RealizarTransaccion(int cedula, int monto, const string &tipoMovimiento)
    {
        Usuarios *usuario = getClientePorCedula(cedula); //  guardo la direccion de memoria del usuario obtenido por la cedula
        if (usuario != nullptr && usuario->EstaConectado())
        { // si existe el usuario y esta conectado entonces , instancio el objeto de transaccion y la inserto en la lista
            Transaccion nuevaTransaccion(to_string(ListaTransacciones.size() + 1), monto, tipoMovimiento);

            if (tipoMovimiento == "Deposito")
            { // si el tipo de movimiento es deposito entonces accedo a la cuenta del usuario y ejecuto el metodo de depositar

                usuario->getCuentaCliente().Depositar(monto);
                cout << "Depósito realizado.\nNuevo saldo: $" << usuario->getCuentaCliente().getSaldoCliente() << endl;
                nuevaTransaccion.setEstadoExitoso("Exitoso");
            }
            else if (tipoMovimiento == "Retiro")
            { // en caso de Retiro verifico que el usuario tenga los fondos suficientes en su cuenta, si da true entonces se realiza la transaccion
                if (usuario->getCuentaCliente().Retirar(monto))
                {
                    cout << "Retiro realizado. Nuevo saldo: $" << usuario->getCuentaCliente().getSaldoCliente() << endl;
                    nuevaTransaccion.setEstadoExitoso("Exitoso");
                }
                else
                { // si da false entonces , el usuario no cuenta con los fondos necesarios
                    cout << "Error: Fondos insuficientes para el retiro." << endl;
                    nuevaTransaccion.setEstadoFallido("Fallido");
                }
            }
            else
            {
                cout << "Error: Tipo de transacción desconocido." << endl;
                nuevaTransaccion.setEstadoFallido("Fallido");
            }
            ListaTransacciones.push_back(nuevaTransaccion); // agrego a la lista la nueva transaccion
        }
        else
        {
            cout << "Error: El usuario no está conectado o no existe." << endl;
        }
    }
    void RealizarTransferencia(Banco &bancoBeneficiario, int cedulaOrigen, string tipoCuenta, string numeroCuenta, int montoTransferir)
    {
        Usuarios *beneficiario = bancoBeneficiario.getClientePorTipoNumero(tipoCuenta, numeroCuenta); // Buscar beneficiario en el banco del beneficiario
        Usuarios *usuarioEnvia = getClientePorCedula(cedulaOrigen);                                   // Buscar el remitente en el banco actual

        if (usuarioEnvia != nullptr && usuarioEnvia->EstaConectado() && beneficiario != nullptr)
        {
            // Creación de la transacción
            Transaccion nuevaTransaccion(to_string(ListaTransacciones.size() + 1), montoTransferir, "Transferencia");

            // Realizar la transferencia
            if (usuarioEnvia->getCuentaCliente().Retirar(montoTransferir))
            {
                beneficiario->getCuentaCliente().Depositar(montoTransferir);
                cout << "Transferencia completada con éxito!" << endl;
                cout << "Saldo actual: $" << usuarioEnvia->getCuentaCliente().getSaldoCliente() << endl;
                nuevaTransaccion.setEstadoExitoso("Exitoso");
            }
            else
            {
                cout << "Error: Fondos insuficientes para realizar la transferencia." << endl;
                nuevaTransaccion.setEstadoFallido("Fallido");
            }

            ListaTransacciones.push_back(nuevaTransaccion); // Agregar transacción a la lista
        }
        else
        {
            cout << "Error: No se puede realizar la transferencia." << endl;
        }
    }

    void HistorialTransacciones() const // recorro el vector de transacciones  mostrando los atributos del objeto
    {
        for (const Transaccion &transaccion : ListaTransacciones)
        {
            cout << " Transaccion ID: " << transaccion.getIdTransaccion()
                 << ", Monto: " << transaccion.getMonto()
                 << ", Tipo transaccion: " << transaccion.getTipoTransaccion();
            if (transaccion.getEstado() == "Exitoso")
                cout << " ,Estado: Exitoso" << endl;
            else
                cout << " ,Estado: Fallido" << endl;
        }
    }

    void ConectarUsuario(int cedula)
    {
        Usuarios *usuario = getClientePorCedula(cedula);
        if (usuario != NULL)
        {
            usuario->Conectar();
            usuarioConectado = usuario;
            // cout << "El usuario: " << usuario->getName() << " " << usuario->getApellido() << " se encuentra conectado." << endl;
        }
        else
        {
            cout << " El usuario no ha sido encontrado... " << endl;
        }
    }
    void UsuarioDesconectado(int cedula)
    {
        Usuarios *usuario = getClientePorCedula(cedula);
        if (usuario != NULL)
        {
            if (usuario->EstaConectado())
            {
                usuario->Desconectar();
                usuarioConectado = nullptr;
            }
            else
            {
                cout << " El usuario ya se encuentra desconectado " << endl;
            }
        }
        else
        {
            cout << "El usuario no se ha encontrado" << endl;
        }
    }
    bool estaConectado()
    {
        return usuarioConectado != nullptr;
    }

    void MostrarSaldoCliente(int cedula)
    {
        Usuarios *usuario = getClientePorCedula(cedula); // obtengo direccion de memoria el obj cliente
        cout << "Saldo total: $" << usuario->getCuentaCliente().getSaldoCliente() << " " << endl;
    }

    void AgregarBancos(const string &nombre, const string &tipo)
    {
        Banco nuevoBanco(nombre, tipo);
        Lista_Bancos.push_back(nuevoBanco);
    }

    void ListadoBancos()
    {
        Banco banco;
        banco.AgregarBancos("Itau", "Nacional");
        banco.AgregarBancos("Santander", "Internacional");
        banco.AgregarBancos("BBVA", "Internacional");
        banco.AgregarBancos("Scotianbank", "Nacional");
    }
    void MostrarBancos() const
    {
        if (Lista_Bancos.empty())
        {
            cout << "No hay Bancos cargados..." << endl;
            return;
        }
        else
        {
            for (const Banco &banco : Lista_Bancos)
            {
                cout << banco.getNombreBanco() << "-" << banco.getTipo() << endl;
            }
        }
    }
    void AltaBanco(const Banco &banco)
    {
        Lista_Bancos.push_back(banco);
    }
};

void MostrarCreditos()
{
    cout << "Créditos del Proyecto Mini Sistema Bancario\n";
    cout << "Desarrollado por: Felipe Masliah\n";
    cout << "Descripción: Un sistema simple de gestión bancaria, que pone en practica mis conocimientos en programación orientada a objetos.\n";
}

void InicializarBancos(Banco &banco)
{
    banco.AgregarBancos("Itau", "Nacional");
    banco.AgregarBancos("Santander", "Internacional");
    banco.AgregarBancos("BBVA", "Internacional");
    banco.AgregarBancos("Scotianbank", "Nacional");
}

void MenuUsuario(Banco &banco, Usuarios *usuario, Banco &bancoPrincipal)
{
    int opcion, cantidad, cedula = usuario->getCedula();
    int op;
    string tipoCuenta, numeroCuenta, nombre_Banco, respuesta;
    Banco banco2;
    do
    {
        cout << "Bienvenido, " << usuario->getName() << endl;
        cout << "1 - Depositar" << endl;
        cout << "2 - Retirar" << endl;
        cout << "3-  Transferencia" << endl;
        cout << "4 - Mostrar Saldo" << endl;
        cout << "5 - Ver Historial de Transacciones" << endl;
        cout << "6 - Desconectar" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
        {
            system("clear");
            cout << "****Deposito****" << endl;
            cout << "Monto de deposito: ";
            cin >> cantidad;
            if (cantidad >= 0)
            {
                banco.ConectarUsuario(cedula); // conecto al usuario al sistema del banco antes de realizar la transaccion
                banco.RealizarTransaccion(cedula, cantidad, "Deposito");
                banco.UsuarioDesconectado(cedula); // luego por temas de seguridad lo desconecto
            }
            else
            {
                cout << "Error: Monto del deposito no puede ser negativo" << endl;
                break;
            }
        }
        break;
        case 2:
        {
            system("clear");
            cout << "***Retiro****" << endl;
            cout << "Monto del retiro: ";
            cin >> cantidad;
            if (cantidad >= 0)
            {
                banco.ConectarUsuario(cedula); // idem con depositar
                banco.RealizarTransaccion(cedula, cantidad, "Retiro");
                banco.UsuarioDesconectado(cedula);
            }
            else
            {
                cout << "Error: Monto a retirar no puede ser negativo" << endl;
                break;
            }
        }
        break;
        case 3:
        {
            system("clear");
            cout << "******Transferencias******" << endl;
            cout << "Nombre del banco destino: ";
            cin >> nombre_Banco;

            // Obtener el banco destino
            Banco *bancoBeneficiario = bancoPrincipal.getBanco(nombre_Banco);
            if (bancoBeneficiario == nullptr)
            {
                cout << "Error: El banco ingresado no existe." << endl;
                break;
            }

            // Pedir detalles de la cuenta destino
            cout << "1- Cuenta corriente(cc)" << endl;
            cout << "2- Caja ahorros(ca)" << endl;
            cout << "Tipo de cuenta del Beneficiario: ";
            cin >> op;

            tipoCuenta = (op == 1) ? "Cuenta corriente" : (op == 2) ? "Caja ahorro"
                                                                    : "";
            if (tipoCuenta.empty())
            {
                cout << "Error: opción no válida." << endl;
                break;
            }

            cout << "Ingrese número de cuenta del destino: ";
            cin >> numeroCuenta;

            // Verificar si la cuenta destino existe en el banco beneficiario
            Usuarios *usuarioDestino = bancoBeneficiario->getClientePorTipoNumero(tipoCuenta, numeroCuenta);
            if (usuarioDestino == nullptr)
            {
                cout << "Error: La cuenta destino no existe en el banco indicado." << endl;
                break;
            }

            // Pedir monto y confirmar la transferencia
            cout << "Ingrese monto de transferencia: $";
            cin >> cantidad;
            if (cantidad >= 0)
            {
                cout << "***Verifique datos****" << endl;
                cout << "Nombre Destino: " << usuarioDestino->getName() << endl;
                cout << "Apellido Destino: " << usuarioDestino->getApellido() << endl;
                cout << "Tipo y Número de Cuenta: " << tipoCuenta << " " << numeroCuenta << endl;
                cout << "Monto a transferir: $" << cantidad << endl;
                cout << "¿Está seguro que los datos son correctos? (yes/no): ";
                cin >> respuesta;

                if (respuesta == "YES" || respuesta == "yes")
                {
                    banco.ConectarUsuario(cedula);
                    banco.RealizarTransferencia(*bancoBeneficiario, cedula, tipoCuenta, numeroCuenta, cantidad);
                    banco.UsuarioDesconectado(cedula);
                }
            }
            else
            {
                cout << "Error: Monto de deposito no pude ser negativo" << endl;
                break;
            }
        }
        break;
        case 4:
            system("clear");
            cout << "Su saldo actual es: $" << usuario->getSaldo() << endl;
            break;
        case 5:
            system("clear");
            cout << "*****Transacciones realizadas*****" << endl;
            banco.HistorialTransacciones();
            break;
        case 6:
            usuario->Desconectar();
            cout << "Has sido desconectado." << endl;
            system("clear");
            break;
        default:
            cout << "Opción no válida, intente de nuevo." << endl;
            break;
        }
    } while (opcion != 6);
}

void MenuPrincipal(Banco &banco)
{
    int opcion;
    do
    {

        cout << "**** Menu Principal ****" << endl;
        cout << "1 - Crear Usuario" << endl; // linea 301 lista de bancos
        cout << "2 - Iniciar Sesion" << endl;
        cout << "3 - Creditos" << endl;
        cout << "4 - Salir" << endl;
        cout << "Ingrese una opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
        {
            string nombre, apellido, pass, numeroCuenta, tipoCuenta, nombre_Banco;
            int cedula;
            system("clear");
            cout << "**** Registro de Usuario Banco ****" << endl;
            cout << "Ingrese nombre de Banco: ";
            cin >> nombre_Banco;

            Banco *bancoEncontrado = banco.getBanco(nombre_Banco);

            if (bancoEncontrado == nullptr)
            {
                cout << "Banco no encontrado. Por favor, intente de nuevo." << endl;
                break;
            }
            else
            {
                int op;
                cout << "Datos de Usuario" << endl;
                cout << "Nombre: ";
                cin >> nombre;
                cout << "Apellido: ";
                cin >> apellido;
                cout << "Cedula: ";
                cin >> cedula;
                // Verificación de la cédula y el número de cuenta
                if (bancoEncontrado->ExisteCedula(cedula))
                {
                    cout << "Error: Ya existe un usuario con esta cédula." << endl;
                    break;
                }
                cout << "Ingrese una contraseña para su cuenta: ";
                cin >> pass;

                cout << "\nDatos de cuenta Bancaria\nSeleccione tipo de cuenta (cc-ca)" << endl;
                cout << "1 - Cuenta corriente" << endl;
                cout << "2 - Caja de ahorros\nIngrese opcion: ";
                cin >> op;

                tipoCuenta = (op == 1) ? "Cuenta corriente" : (op == 2) ? "Caja ahorro"
                                                                        : "";

                if (tipoCuenta.empty())
                {
                    cout << "Error: opción no válida" << endl;
                    break;
                }

                cout << "Ingrese número de cuenta: ";
                cin >> numeroCuenta;
                if (bancoEncontrado->ExisteNumeroCuenta(numeroCuenta))
                {
                    cout << "Error: Ya existe un número de cuenta similar." << endl;
                    break;
                }

                CuentaCliente cuenta(numeroCuenta, tipoCuenta, 0);
                Usuarios nuevoUsuario(nombre, apellido, cedula, cuenta, pass);
                bancoEncontrado->AltaUsuario(nuevoUsuario);
                system("clear");
                cout << "Usuario creado exitosamente" << endl;
                // Imprimir detalles del usuario
                cout << "\nDetalles del Usuario Creado:" << endl;
                cout << "Nombre: " << nuevoUsuario.getName() << endl;
                cout << "Apellido: " << nuevoUsuario.getApellido() << endl;
                cout << "Cedula: " << nuevoUsuario.getCedula() << endl;
                cout << "Contraseña: " << pass << " (Mantenga su contraseña segura)" << endl;

                // Detalles de la cuenta bancaria
                CuentaCliente cuentaUsuario = nuevoUsuario.getCuentaCliente();
                cout << "Datos de Cuenta Bancaria:" << endl;
                cout << "Tipo de Cuenta: " << cuentaUsuario.getTipoCuenta() << endl;
                cout << "Número de Cuenta: " << cuentaUsuario.getNumeroCuenta() << endl;
                cout << "Saldo Inicial: $" << cuentaUsuario.getSaldoCliente() << endl;
                break;
            }
        }

        case 2:
        {
            int cedula;
            string pass, nombreBanco;
            system("clear");
            cout << "Ingrese nombre del Banco: ";
            cin >> nombreBanco;
            Banco *bancoSeleccionado = banco.getBanco(nombreBanco);
            if (bancoSeleccionado == nullptr)
            {
                cout << "Error: El banco no existe.." << endl;
                break;
            }
            else
            {
                cout << "Ingrese su cédula: ";
                cin >> cedula;
                cout << "Ingrese su contraseña: ";
                cin >> pass;

                Usuarios *usuario = bancoSeleccionado->getClientePorCedula(cedula); // retorna el usuario que se encuentra en el banco seleccionado
                if (usuario != nullptr && usuario->getPass() == pass)
                {
                    usuario->Conectar();
                    MenuUsuario(*bancoSeleccionado, usuario, banco);
                }
                else
                {
                    cout << "Cédula o contraseña incorrecta." << endl;
                }
                break;
            }
        }
        break;
        case 3:
            system("clear");
            MostrarCreditos();
            break;
        case 4:
            system("clear");
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opción no válida, intente de nuevo." << endl;
            break;
        }
    } while (opcion != 4);
}

int main()
{
    Banco bancoMain;
    InicializarBancos(bancoMain); // Inicializar algunos bancos
    MenuPrincipal(bancoMain);
    return 0;
}

/*
   **********Problemas Solucionados******************
   -Se soluciono error de Transferencias a bancos diferentes
   -Se soluciono error al logearse con usuarios de bancos diferentes
  **********Detalles a pulir*************************************
  -Si el usuario coloca su propia cuenta para las transferencias , debe emitirt un mensaje de error "No se puede transferirse a cuenta propia, dirigirse a deposito",
  -Modificar la funcion de historial de transacciones pero para que retorne solo las transacciones del usuario actual y no todas las transacciones del banco actual



*/