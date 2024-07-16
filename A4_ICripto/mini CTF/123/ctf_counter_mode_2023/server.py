#!/usr/bin/env python3
import argparse
import socket
import socketserver

from counter_mode import AESCounterMode

FLAG: str = ""
MAX_REQUESTS = -1

MENU = """
┌───────── MENU ─────────┐
│ [1] Show FLAG          │
│ [2] Encrypt            │
│ [3] Exit               │
└────────────────────────┘
"""


class ConnectionHandler(socketserver.StreamRequestHandler):
    def __init__(self, *args, **kwargs) -> None:
        self._key = AESCounterMode.generate_key()
        self._nonce = AESCounterMode.generate_nonce()
        self._encrypt = AESCounterMode(self._key, self._nonce)
        super().__init__(*args, **kwargs)

    def _send(self, message: str) -> None:
        self.wfile.write(f"{message}\n".encode())

    def _get_user_input(self, prompt: str) -> str:
        self._send(prompt)
        return self.rfile.readline().decode()

    def handle(self) -> None:
        # Disable Nagle's algorithm to improve performance
        self.request.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)

        self._send(MENU)

        for _ in range(MAX_REQUESTS):
            choice = self._get_user_input("Enter your choice:").strip()

            if choice == "1":
                enc = self._encrypt(FLAG.encode()).hex()
                self._send(f"Encrypted FLAG: {enc}")

            elif choice == "2":
                plaintext = self._get_user_input("Enter Plaintext:").strip()
                ciphertext = self._encrypt(plaintext.encode()).hex()
                if ciphertext == FLAG.encode().hex():
                    self._send(f"Please don't cheat. Decrypt it yourself!")
                else:
                    self._send(f"Encrypted plaintext: {ciphertext}")

            else:
                break

        self._send("Bye!")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--host",
        type=str,
        default="localhost",
        help="The host (default: localhost)",
    )
    parser.add_argument(
        "--port",
        type=int,
        default=9776,
        help="The port (default: 80)",
    )
    parser.add_argument(
        "--flagpath",
        type=str,
        default="flag.txt",
        help="The path to the flag (default: flag.txt)",
    )
    parser.add_argument(
        "--max-requests",
        type=int,
        default=500,
        help="Limit the number of requests per connection (default: 500)",
    )
    return parser.parse_args()


def main(args: argparse.Namespace) -> None:
    global FLAG, MAX_REQUESTS
    with open(args.flagpath, "rt", encoding="utf8") as f:
        FLAG = f.read().strip()
    MAX_REQUESTS = args.max_requests

    with socketserver.ForkingTCPServer(
        (args.host, args.port), ConnectionHandler
    ) as server:
        # Activate the server; this will keep running until you
        # interrupt the program with Ctrl-C
        server.serve_forever()


if __name__ == "__main__":
    main(parse_args())
