import os
from lib.common import helpers, messages

PAYLOAD_DIR = os.path.join(os.path.dirname(__file__), "payloads")

def list_payloads():
    names = []
    for f in os.listdir(PAYLOAD_DIR):
        if f.endswith(".py") and not f.startswith("__"):
            names.append(f.replace(".py", ""))
    return sorted(names)

class Evasion:
    def main_menu(self):
        while True:
            print("\n--- Yerli-Evasion ---")
            print("  1) List payloads")
            print("  2) Generate payload")
            print("  0) Back")
            c = input("evasion> ").strip()
            if c == "1":
                for p in list_payloads():
                    print(f"    python/{p}")
            elif c == "2":
                self.interactive_generate()
            elif c == "0":
                return

    def interactive_generate(self):
        print("[*] Mevcut payload'lar:", ", ".join(list_payloads()))
        name = input("payload name> ").strip()
        lhost = input("LHOST> ").strip() or "127.0.0.1"
        lport = input("LPORT> ").strip() or "4444"
        self.generate(name, {"LHOST": lhost, "LPORT": lport})

    def command_line(self, args):
        opts = {}
        if args.c:
            for kv in args.c:
                if "=" in kv:
                    k, v = kv.split("=", 1)
                    opts[k.upper()] = v
        if args.p == "list" or not args.p or args.list_payloads:
            print("[*] Payloads:", ", ".join(list_payloads()))
            return
        self.generate(args.p.split("/")[-1], opts, args.o)

    def generate(self, name, opts, output_name=None):
        try:
            mod = importlib.import_module(f"lib.tools.Evasion.payloads.{name}")
            payload = getattr(mod, "Payload")()
        except ModuleNotFoundError:
            print(f"[!] Payload bulunamadı: {name}")
            return
        out = payload.generate(opts)
        output_name = output_name or f"payload_{helpers.random_string()}"
        out_dir = helpers.get_output_dir()
        src = helpers.write_file(os.path.join(out_dir, output_name + ".py"), out)
        print(f"[+] Source    : {src}")
        print(f"[*] Derleme   : pyinstaller --onefile --noconsole {src}")
        print(f"[*] SHA256    : {helpers.sha256_file(src)}")
