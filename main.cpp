#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// colores para los mensajes importantes
const string rojo = "\033[31m";
const string verde = "\033[32m";
const string DEFAULT = "\033[0m";

class CuentaCliente
{
private:
    string NumeroCuenta;
    string Tipocuenta; // cc , ca
    int Saldo_cliente; // saldo total del cliente

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
    bool Activo;
    vector<CuentaCliente> ListaCuentas; // almacena las cuentas que tiene un usuario

public:
    Usuarios() {}
    Usuarios(string nombre, string apellido, int dni, string password) : Nombre(nombre), Apellido(apellido), Cedula(dni), Pass(password), Conectado(false), Activo(true) {}
    Usuarios(string nombre, string apellido, int dni, CuentaCliente cuenta, string p) : Nombre(nombre), Apellido(apellido), Cedula(dni), Cuenta(cuenta), Pass(p), Conectado(false), Activo(true) {}

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
    void DarAlta() { Activo = true; }
    void DarBaja() { Activo = false; }

    bool estaActivo() const { return Activo; }
    // estado de usuario
    void Conectar() { Conectado = true; }
    void Desconectar() { Conectado = false; }

    void AgregarCuenta(const CuentaCliente &nuevaCuenta)
    {
        ListaCuentas.push_back(nuevaCuenta);
    }
    vector<CuentaCliente> &ObtenerCuentas()
    { // obtengo las cuentas que tiene un cliente
        return ListaCuentas;
    }

    // Versión const para lectura
    const vector<CuentaCliente> &ObtenerCuentas() const
    {
        return ListaCuentas;
    }

    size_t getCantidadCuentas() const // obtengo la cantidad de cuentas
    {
        return ObtenerCuentas().size();
    }
};

class Transaccion
{
private:
    string idTransaccion;
    int Monto;
    string Estado_Transaccion; // Fallido o Exitoso
    string TipoTrans;          // tipo transaccion: retiro o deposito
    int cedulaCliente;

public:
    Transaccion(string id, int cantidad, string tipo, int dni) : idTransaccion(id), Monto(cantidad), TipoTrans(tipo), cedulaCliente(dni) {}

    string getIdTransaccion() const { return idTransaccion; }
    int getMonto() const { return Monto; }
    string getTipoTransaccion() const { return TipoTrans; }
    string getEstado() const { return Estado_Transaccion; }

    int getCedulaCliente() const { return cedulaCliente; }
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
        try{
        for (Usuarios &user : ListaClientes)
        {
            //   cout << "Revisando usuario con cedula: " << user.getCedula() << endl;
            if (user.getCedula() == cedula)
            {
                //      cout << "Usuario encontrado." << endl;
                return &user;
            }
        }
        }catch(const exception &e){
         cout << rojo << "Usuario no encontrado."  << e.what()<< DEFAULT << endl;
         throw e;
        }
        return nullptr;
    }
    Usuarios *getClientePorTipoNumero(string tipo, string numero) {
    
    for (Usuarios &user : ListaClientes) {
        for (CuentaCliente &cuenta : user.ObtenerCuentas()) {
            if (cuenta.getNumeroCuenta() == numero && cuenta.getTipoCuenta() == tipo) {
                return &user;
            }
        }
    }
    
    return nullptr; // Si no se encuentra la cuenta
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

    void RealizarTransaccion(int cedula, int monto, const string &tipoMovimiento, size_t indexCuenta)
    {
        Usuarios *usuario = getClientePorCedula(cedula);
        if (usuario != nullptr && usuario->EstaConectado() && usuario->estaActivo())
        {
            if (indexCuenta >= 0 && indexCuenta < usuario->ObtenerCuentas().size()) // comparison of unsigned expression in >= 0 is always true => warning
            {
                // Asumimos que ObtenerCuentas() devuelve un vector no constante para poder modificar la cuenta seleccionada.
                CuentaCliente &cuentaSeleccionada = usuario->ObtenerCuentas()[indexCuenta];
                Transaccion nuevaTransaccion(to_string(ListaTransacciones.size() + 1), monto, tipoMovimiento, cedula);

                if (tipoMovimiento == "Deposito")
                {
                    cuentaSeleccionada.Depositar(monto);
                    cout << verde << "Depósito realizado en la cuenta " << cuentaSeleccionada.getNumeroCuenta() << ".\nNuevo saldo: $" << cuentaSeleccionada.getSaldoCliente() << DEFAULT << endl;
                    nuevaTransaccion.setEstadoExitoso("Exitoso");
                }
                else if (tipoMovimiento == "Retiro")
                {
                    if (cuentaSeleccionada.Retirar(monto))
                    {
                        cout << verde << "Retiro realizado de la cuenta " << cuentaSeleccionada.getNumeroCuenta() << ". Nuevo saldo: $" << cuentaSeleccionada.getSaldoCliente() << DEFAULT << endl;
                        nuevaTransaccion.setEstadoExitoso("Exitoso");
                    }
                    else
                    {
                        cout << rojo << "Error: Fondos insuficientes para el retiro de la cuenta " << cuentaSeleccionada.getNumeroCuenta() << "." << DEFAULT << endl;
                        nuevaTransaccion.setEstadoFallido("Fallido");
                    }
                }
                else
                {
                    cout << rojo << "Error: Tipo de transacción desconocido." << DEFAULT << endl;
                    nuevaTransaccion.setEstadoFallido("Fallido");
                }
                ListaTransacciones.push_back(nuevaTransaccion);
            }
            else
            {
                cout << rojo << "Error: Índice de cuenta no válido." << DEFAULT << endl;
            }
        }
        else
        {
            cout << rojo << "Error: El usuario no existe, no está conectado, o está dado de baja." << DEFAULT << endl;
        }
    }

    void RealizarTransferencia(Banco &bancoBeneficiario, int cedulaOrigen, string tipoCuenta, string numeroCuenta, int montoTransferir, size_t indexCuentaOrigen)
    {
        Usuarios *usuarioEnvia = getClientePorCedula(cedulaOrigen);
        if (usuarioEnvia == nullptr || !usuarioEnvia->EstaConectado() || !usuarioEnvia->estaActivo())
        {
            cout << rojo << "Error: Usuario origen no válido o no conectado." << DEFAULT << endl;
            return;
        }

        if (indexCuentaOrigen >= usuarioEnvia->ObtenerCuentas().size())
        {
            cout << rojo << "Error: Índice de cuenta origen no válido." << DEFAULT << endl;
            return;
        }

        CuentaCliente &cuentaOrigen = usuarioEnvia->ObtenerCuentas()[indexCuentaOrigen];

        Usuarios *beneficiario = bancoBeneficiario.getClientePorTipoNumero(tipoCuenta, numeroCuenta);

        Transaccion nuevaTransaccion(to_string(ListaTransacciones.size() + 1), montoTransferir, "Transferencia", beneficiario->getCedula()); // transaccion para el beneficiario
        Transaccion nuevaTransaccion1(to_string(ListaTransacciones.size() + 1), montoTransferir, "Transferencia", cedulaOrigen);             // transaccion para el depositante
        if (beneficiario == nullptr || !beneficiario->estaActivo())
        {
            cout << rojo << "Error: Cuenta de destino no válida o beneficiario inactivo." << DEFAULT << endl;
            return;
        }

        bool cuentaEncontrada = false;
        for (CuentaCliente &cuentaDestino : beneficiario->ObtenerCuentas())
        {
            if (cuentaDestino.getTipoCuenta() == tipoCuenta && cuentaDestino.getNumeroCuenta() == numeroCuenta)
            {
                if (cuentaOrigen.Retirar(montoTransferir))
                {
                    cuentaDestino.Depositar(montoTransferir);
                    cout << verde << "Transferencia completada con éxito. Monto: $" << montoTransferir << DEFAULT << endl;
                    cout << "Saldo actual: $" << cuentaOrigen.getSaldoCliente() << DEFAULT << endl; // obtengo el saldo total luego del retiro de la cuenta seleccionada
                    nuevaTransaccion.setEstadoExitoso("Exitoso");
                    nuevaTransaccion1.setEstadoExitoso("Exitoso");
                    cuentaEncontrada = true;

                    ListaTransacciones.push_back(nuevaTransaccion1);                  // Agregar transacción a la lista
                    bancoBeneficiario.ListaTransacciones.push_back(nuevaTransaccion); // Agregar transacción a la lista del usario beneficiario
                    break;
                }
                else
                {
                    cout << rojo << "Error: Fondos insuficientes para realizar la transferencia." << DEFAULT << endl;
                    nuevaTransaccion.setEstadoFallido("Fallido");
                    nuevaTransaccion1.setEstadoFallido("Fallido");

                    return;
                }
            }
        }

        if (!cuentaEncontrada)
        {
            cout << rojo << "Error: La cuenta de destino especificada no fue encontrada." << DEFAULT << endl;
        }
    }

    void HistorialTransacciones(int cedulaCliente) const // recorro el vector de transacciones  mostrando los atributos del objeto
    {
        bool encontrado = false;

        for (const Transaccion &transaccion : ListaTransacciones)
        {
            if (transaccion.getCedulaCliente() == cedulaCliente)
            {
                encontrado = true;
                cout << " Transaccion ID: " << transaccion.getIdTransaccion()
                     << ", Monto: " << transaccion.getMonto()
                     << ", Tipo transaccion: " << transaccion.getTipoTransaccion();
                if (transaccion.getEstado() == "Exitoso")
                    cout << verde << " ,Estado: Exitoso" << DEFAULT << endl;
                else
                    cout << rojo << " ,Estado: Fallido" << DEFAULT << endl;
            }
        }
        if (!encontrado)
        {
            cout << rojo << "No se encontraron transacciones realizadas por el usuario" << DEFAULT << endl;
        }
    }

    void MostrarCuentasDelUsuario(const Usuarios &usuario)
    {
        auto &cuentas = usuario.ObtenerCuentas();
        for (auto &cuenta : cuentas)
        {
            cout << "Número de Cuenta: " << cuenta.getNumeroCuenta() << ", "
                 << "Tipo: " << cuenta.getTipoCuenta() << ", "
                 << verde << "Saldo: $" << cuenta.getSaldoCliente() << DEFAULT << endl;
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
            cout << rojo << " El usuario no ha sido encontrado o esta dado de baja... " << DEFAULT << endl;
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
            cout << rojo << "No hay Bancos cargados..." << DEFAULT << endl;
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

bool salir(const string &salida)
{ // retorna true si el usuario ingreso z para salir
    return (salida == "z" || salida == "Z");
}
void MenuUsuario(Banco &banco, Usuarios *usuario, Banco &bancoPrincipal)
{
    int opcion, cantidad, cedula = usuario->getCedula();
    int op, cantidad_cuentas = 1;
    string tipoCuenta, numeroCuenta, nombre_Banco, respuesta;
    Banco banco2;

    do
    {
        cout << "Usuario actual (" << usuario->getName() << ")  Banco " << banco.getNombreBanco() << "" << endl;
        cout << "1 - Operaciones" << endl;
        cout << "2 - Mis Cuentas" << endl;
        cout << "3 - Opciones de Usuario" << endl;
        cout << "4 - Historial de Transacciones" << endl;
        cout << "5 - Cerrar sesion" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            system("clear");
            cout << "****** Operaciones ******" << endl;
            cout << "1-Depositos(cuenta propia)" << endl;
            cout << "2-Retiros" << endl;
            cout << "3-Transferencias" << endl;
            cout << "Ingrese operacion: ";
            cin >> op;

            switch (op)
            {
            case 1:
            {
                system("clear");
                size_t seleccion;
                cout << "****Deposito****" << endl;
                cout << "Seleccione la cuenta para el deposito: " << endl;
                for (size_t i = 0; i < usuario->ObtenerCuentas().size(); i++)
                {
                    cout << i + 1 << ") Cuenta:  " << usuario->ObtenerCuentas()[i].getTipoCuenta() << " " << usuario->ObtenerCuentas()[i].getNumeroCuenta() << endl;
                }
                cout << "indice: ";
                cin >> seleccion;
                cout << "Monto de deposito: ";
                cin >> cantidad;
                system("clear");
                if (cantidad >= 0)
                {

                    banco.ConectarUsuario(cedula); // conecto al usuario al sistema del banco antes de realizar la transaccion
                    banco.RealizarTransaccion(cedula, cantidad, "Deposito", seleccion - 1);
                    banco.UsuarioDesconectado(cedula); // luego por temas de seguridad lo desconecto
                }
                else
                {
                    cout << rojo << "Error: Monto del deposito no puede ser negativo" << DEFAULT << endl;
                    break;
                }
            }
            break;
            case 2:
            {
                system("clear");
                size_t seleccion;
                cout << "***Retiro****" << endl;
                cout << "Seleccione la cuenta para retirar: " << endl;
                for (size_t i = 0; i < usuario->ObtenerCuentas().size(); i++)
                {
                    cout << i + 1 << ") Cuenta:  " << usuario->ObtenerCuentas()[i].getTipoCuenta() << " " << usuario->ObtenerCuentas()[i].getNumeroCuenta() << endl;
                }
                cout << "indice: ";
                cin >> seleccion;
                cout << "Monto del retiro: ";
                cin >> cantidad;
                system("clear");
                if (cantidad >= 0)
                {
                    banco.ConectarUsuario(cedula); // idem con depositar
                    banco.RealizarTransaccion(cedula, cantidad, "Retiro", seleccion - 1);
                    banco.UsuarioDesconectado(cedula);
                }
                else
                {
                    cout << rojo << "Error: Monto a retirar no puede ser negativo" << DEFAULT << endl;
                    break;
                }
            }
            break;
            case 3:
            { // arreglar transferencia, pensar como seleccionar  de que cuenta se va a retirar los fondos para depositar al destino
                system("clear");
                size_t seleccion;
                cout << "******Transferencias******" << endl;
                cout << "Nombre del banco destino: ";
                cin >> nombre_Banco;

                // Obtener el banco destino
                Banco *bancoBeneficiario = bancoPrincipal.getBanco(nombre_Banco);
                if (bancoBeneficiario == nullptr)
                {
                    cout << rojo << "Error: El banco ingresado no existe." << DEFAULT << endl;
                    break;
                }
                // selecciono la cuenta donde quiero que salgan los fondos
                cout << "Seleccione la cuenta de origen: " << endl;
                for (size_t i = 0; i < usuario->ObtenerCuentas().size(); i++)
                {
                    cout << i + 1 << ") Cuenta:  " << usuario->ObtenerCuentas()[i].getTipoCuenta() << " " << usuario->ObtenerCuentas()[i].getNumeroCuenta() << endl;
                }
                cout << "Índice de la cuenta: ";
                cin >> seleccion;
                seleccion--;

                if (seleccion < 0 || seleccion >= usuario->ObtenerCuentas().size())
                {
                    cout << rojo << "Error: Selección fuera de rango." << DEFAULT << endl;
                    break;
                }
               
                // Pedir detalles de la cuenta destino
                cout << "Cuenta del destino: " << endl;
                cout << "1- Cuenta corriente(cc)" << endl;
                cout << "2- Caja ahorros(ca)" << endl;
                cout << "Tipo de cuenta del Beneficiario: ";
                cin >> op;

                tipoCuenta = (op == 1) ? "Cuenta corriente" : (op == 2) ? "Caja ahorro"
                                                                        : "";
                if (tipoCuenta.empty())
                {
                    cout << rojo << "Error: opción no válida." << DEFAULT << endl;
                    break;
                }

                cout << "Ingrese número de cuenta del destino: ";
                cin >> numeroCuenta;

                // Verificar si la cuenta destino existe en el banco beneficiario
                Usuarios *usuarioDestino = bancoBeneficiario->getClientePorTipoNumero(tipoCuenta, numeroCuenta);
                if (usuarioDestino == nullptr)
                {
                    cout << rojo << "Error: La cuenta destino no existe en el banco indicado." << DEFAULT << endl;
                    break;
                }

                // Pedir monto y confirmar la transferencia
                cout << "Ingrese monto de transferencia: $";
                cin >> cantidad;
                system("clear");
                if (cantidad >= 0)
                {
                    cout << "***Verifique datos****" << endl;
                    cout << "Cuenta de origen:  " << usuario->ObtenerCuentas()[seleccion].getNumeroCuenta() << endl;
                    cout << "Banco destino: " << bancoBeneficiario->getNombreBanco() << endl;
                    cout << "Nombre Destino: " << usuarioDestino->getName() << endl;
                    cout << "Apellido Destino: " << usuarioDestino->getApellido() << endl;
                    cout << "Tipo y Número de Cuenta: " << tipoCuenta << " " << numeroCuenta << endl;
                    cout << "Monto a transferir: $" << cantidad << endl;
                    cout << "¿Está seguro que los datos son correctos? (yes/no): ";
                    cin >> respuesta;

                    if (respuesta == "YES" || respuesta == "yes")
                    {
                        banco.ConectarUsuario(cedula);
                        banco.RealizarTransferencia(*bancoBeneficiario, cedula, tipoCuenta, numeroCuenta, cantidad, seleccion);
                        banco.UsuarioDesconectado(cedula);
                    }
                }
                else
                {
                    cout << rojo << "Error: Monto de deposito no pude ser negativo" << DEFAULT << endl;
                    break;
                }
            }
            break;
            default:
                cout << "Error: opcion invalida" << endl;
                break;
            }

            break;
        case 2:
        { // arreglar, validar que no se repitan los numeros de cuenta independientemente que sean de cajas diferentes cc o ca
            system("clear");
            string res;
            cout << "******* Mis Cuentas ***********" << endl;
            banco.MostrarCuentasDelUsuario(*usuario);
            cout << "Crear Nueva cuenta? (yes/no)" << endl;
            cout << verde;
            cin >> res;
            cout << DEFAULT;
            system("clear");
            if (res == "yes" || res == "YES")
            {
                string tipo, numero;
                int op;
                cout << "****** Nueva Cuenta ******* " << endl;
                if (cantidad_cuentas > 0)
                {
                    cout << "Tipo de cuenta(cc-ca)" << endl;
                    cout << "1-Cuenta corrriente" << endl;
                    cout << "2-Caja de ahorro" << endl;
                    cout << "Ingrese opcion: ";
                    cin >> op;

                    tipo = (op == 1) ? "Cuenta corriente" : (op == 2) ? "Caja ahorro"
                                                                      : " ";

                    if (tipo.empty())
                    {
                        cout << "Opcion invalida";
                        break;
                    }

                    cout << "Número de cuenta: ";
                    cin >> numero;
                    if (banco.ExisteNumeroCuenta(numero))
                    {
                        cout << rojo << "Error: El número ingresado ya existe" << DEFAULT << endl;
                        break;
                    }
                    else
                    {
                        system("clear");
                        string res;
                        cout << "*****Verifique datos de la nueva cuenta *****" << endl;
                        cout << " " << tipo << " " << numero << endl;
                        cout << "Si estan los datos correctos (yes/no): ";
                        cin >> res;
                        system("clear");
                        if (res == "YES" || res == "yes")
                        {
                            CuentaCliente nuevaCuenta(numero, tipo, 0);
                            usuario->AgregarCuenta(nuevaCuenta);
                            cout << verde << "La cuenta se ha creado exitosamente! " << DEFAULT << endl;
                            cantidad_cuentas--; // luego de crear decremento la cantidad de cuentas permitidas
                        }
                    }
                }
                else
                {
                    system("clear");
                    cout << rojo << "Error: se ha llegado al maximo de cuentas, unicamente 2." << DEFAULT << endl;
                    break;
                }
            }
        }
        break;
        case 3:
        {
            system("clear");

            cout << "******** Opciones de Usuario ************ " << endl;
            cout << " 1- Cambiar contraseña" << endl;
            cout << " 2- Ver mis datos" << endl;
            cout << " 3- Dar de baja" << endl;
            cout << " 4- Dar de Alta" << endl;
            cout << "Ingrese opcion: ";
            cin >> op;

            if (op == 1)
            {
                string pass1, pass2, pass3;
                cout << "Cambiar la contrasenia del usuario" << endl;
                cout << "Ingrese su contraseña actual: ";
                cin >> pass1;
                if (usuario->getPass() == pass1)
                {
                    cout << "Ingrese nueva contraseña: ";
                    cin >> pass2;
                    cout << "Ingrese nuevamente la contraseña: ";
                    cin >> pass3;
                    if (pass2 == pass3)
                    {
                        usuario->setPassword(pass2); // actualizo la contrasenia
                        cout << verde << "La contraseña ha sido actualizada exitosamente" << DEFAULT << endl;
                    }
                }
            }
            else if (op == 2)
            {
                cout << "****** Datos de Usuario *********" << endl;
                // banco.mostrarInfoUsuario(cedula);
            }
            else if (op == 3)
            {
                string passUser, res;
                cout << "Esta seguro que desea darse de baja? (yes/no): ";
                cin >> res;
                if (res == "yes")
                {
                    cout << "Ingrese su contrasenia: ";
                    cin >> passUser;
                    if (usuario->getPass() == passUser)
                    {
                        system("clear");
                        usuario->DarBaja();
                        cout << verde << "Usuario dado de baja, ya no podra realizar operaciones ni recibir" << DEFAULT << endl;
                        cout << "Si desea darse de alta, dirigase a Opciones de usuario opcion 4" << endl;
                    }
                }
            }
            else if (op == 4)
            {
                usuario->DarAlta();
                cout << verde << "Usuario dado Alta nuevamente" << DEFAULT << endl;
            }
            else
            {
                cout << rojo << "Error: opcion invalida" << DEFAULT << endl;
                break;
            }
        }
        break;
        case 4:
            system("clear");
            cout << "*****Transacciones realizadas*****" << endl;
            banco.HistorialTransacciones(cedula);
            break;
        case 5:
            usuario->Desconectar();
            cout << "Has sido desconectado." << endl;
            system("clear");
            break;
        default:
            cout << "Opción no válida, intente de nuevo." << endl;
            break;
        }
    } while (opcion != 5);
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
                cout << rojo << "Banco no encontrado. Por favor, intente de nuevo." << DEFAULT << endl;
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
                    cout << rojo << "Error: Ya existe un usuario con esta cédula." << DEFAULT << endl;
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
                    cout << rojo << "Error: opción no válida" << DEFAULT << endl;
                    break;
                }

                cout << "Ingrese número de cuenta: ";
                cin >> numeroCuenta;
                if (bancoEncontrado->ExisteNumeroCuenta(numeroCuenta))
                {
                    cout << rojo << "Error: Ya existe un número de cuenta similar." << DEFAULT << endl;
                    break;
                }

                CuentaCliente cuenta(numeroCuenta, tipoCuenta, 0);
                Usuarios nuevoUsuario(nombre, apellido, cedula, cuenta, pass);
                nuevoUsuario.AgregarCuenta(cuenta); // agrego la cuenta creada por defecto a la lista de cuentas del cliente
                bancoEncontrado->AltaUsuario(nuevoUsuario);
                system("clear");
                cout << " ------------------------------------------" << endl;
                cout << verde << "Usuario creado exitosamente" << endl;
                // Imprimir detalles del usuario
                cout << "Detalles del Usuario Creado:" << endl;
                cout << "Nombre: " << nuevoUsuario.getName() << endl;
                cout << "Apellido: " << nuevoUsuario.getApellido() << endl;
                cout << "Cedula: " << nuevoUsuario.getCedula() << endl;
                cout << "Contraseña: " << pass << " (Mantenga su contraseña segura)" << endl;

                // Detalles de la cuenta bancaria
                CuentaCliente cuentaUsuario = nuevoUsuario.getCuentaCliente();
                cout << "Datos de Cuenta Bancaria:" << endl;
                cout << " Banco: " << bancoEncontrado->getNombreBanco() << endl;
                cout << "Tipo de Cuenta: " << cuentaUsuario.getTipoCuenta() << endl;
                cout << "Número de Cuenta: " << cuentaUsuario.getNumeroCuenta() << endl;
                cout << "Saldo Inicial: $" << cuentaUsuario.getSaldoCliente() << DEFAULT << endl;
                cout << "----------------------------------------------------------------" << endl;
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
                    system("clear");
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
    system("clear");
    InicializarBancos(bancoMain); // Inicializar algunos bancos
    MenuPrincipal(bancoMain);
    return 0;
}

/*
  -Se modifico el codigo para que los usuarios puedan crear multiples cuentas, por ahora solo 2.
   para esto, se modifico la logica de las operaciones bancarias para seleccionar las cuentas donde se necesitan operar.
   - se modifico el menu de usuario y se organizo 
   - se modifico el historial de transacciones ahora solo mostrara las transacciones realizadas por los usuarios
   -Se agrego las opciones de usuario para cambiar contrasenia , darse de baja y de alta nuevamente
   -se modifico el ver saldo, ahora muestra todas las cuentas con sus respectivos saldos

   /-Falta
   - falta validar la unicidad de las cuentas del usuario que no se repitan (importante)
   -falta organizar el menu principal
   -falta eliminar redundancias y codigo no utilizado
   -Falta agregar una forma de salir de las operaciones para regresar al menu(importante)

  - se corrigieron errores en transferencia: no detectaba las cuentas secundarias(segmentation fault), no se actualizaba el saldo al beneficiario, no detectaba las cuentas del envio(segmentation)
    
*/