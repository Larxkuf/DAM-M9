import argparse
import getpass
from cryptography.hazmat.primitives.asymmetric import rsa, dsa, ec
from cryptography.hazmat.primitives import serialization, hashes
from cryptography import x509
from cryptography.x509.oid import NameOID
import os

def generate_keys(algorithm):
    if algorithm == "RSA":
        private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
    elif algorithm == "DSA":
        private_key = dsa.generate_private_key(key_size=2048)
    elif algorithm == "EC":
        private_key = ec.generate_private_key(ec.SECP256R1())
    else:
        raise ValueError("Algoritmo no soportado")
    return private_key, private_key.public_key()

def save_keys(keystore, alias, private_key, public_key, password):
    private_key_path = f"{keystore}_{alias}_private.pem"
    public_key_path = f"{keystore}_{alias}_public.pem"
    
    with open(private_key_path, "wb") as f:
        f.write(private_key.private_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PrivateFormat.PKCS8,
            encryption_algorithm=serialization.BestAvailableEncryption(password.encode())
        ))
    
    with open(public_key_path, "wb") as f:
        f.write(public_key.public_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PublicFormat.SubjectPublicKeyInfo
        ))
    
    print(f"Claves guardadas en {private_key_path} y {public_key_path}")

def load_private_key(keystore, alias, password):
    private_key_path = f"{keystore}_{alias}_private.pem"
    if not os.path.exists(private_key_path):
        print("Keystore no encontrado")
        return None
    with open(private_key_path, "rb") as f:
        return serialization.load_pem_private_key(f.read(), password=password.encode())

def create_csr(private_key, common_name):
    csr = x509.CertificateSigningRequestBuilder().subject_name(
        x509.Name([x509.NameAttribute(NameOID.COMMON_NAME, common_name)])
    ).sign(private_key, hashes.SHA256())
    csr_path = "request.csr"
    with open(csr_path, "wb") as f:
        f.write(csr.public_bytes(serialization.Encoding.PEM))
    print(f"CSR guardado en {csr_path}")

def list_keystore(keystore):
    files = [f for f in os.listdir() if f.startswith(keystore)]
    if not files:
        print("Keystore no encontrado")
        return
    print(f"Keystore {keystore} contiene: {', '.join(files)}")

def show_help():
    print("Use 'pykey -?, -h, or --help' for this help message")
    print("Use 'pykey -command_name --help' for usage of command_name.")

def main():
    while True:
        print("\nKeytool Python - Opciones:")
        print("1. Generar un par de claves    -> pykey -genkeypair")
        print("2. Generar CSR                 -> pykey -certreq")
        print("3. Listar claves en keystore   -> pykey -list")
        print("4. Mostrar ayuda               -> pykey -help")
        print("5. Salir                       -> pykey -exit")
        opcion = input("Ingrese el comando (por ejemplo, pykey -list): ")
        
        args = opcion.split()
        if len(args) < 2 or args[0] != "pykey":
            print("Comando no válido. Debe comenzar con 'pykey'")
            continue
        
        comando = args[1]
        if comando in ["-?", "-h", "--help"]:
            show_help()
        elif comando == "-genkeypair":
            keystore = input("Ingrese el nombre del keystore: ")
            alias = input("Ingrese el alias de la clave: ")
            keyalg = input("Ingrese el algoritmo (RSA, DSA, EC) [RSA]: ") or "RSA"
            storepass = getpass.getpass("Ingrese la contraseña del keystore: ")
            private_key, public_key = generate_keys(keyalg)
            save_keys(keystore, alias, private_key, public_key, storepass)
        
        elif comando == "-certreq":
            keystore = input("Ingrese el nombre del keystore: ")
            alias = input("Ingrese el alias de la clave: ")
            storepass = getpass.getpass("Ingrese la contraseña del keystore: ")
            private_key = load_private_key(keystore, alias, storepass)
            if private_key:
                create_csr(private_key, alias)
        
        elif comando == "-list":
            keystore = input("Ingrese el nombre del keystore: ")
            list_keystore(keystore)
        
        elif comando == "-exit":
            print("Saliendo...")
            break
        else:
            print("Comando no reconocido. Use 'pykey -help' para ver las opciones disponibles.")

if __name__ == "__main__":
    main()
