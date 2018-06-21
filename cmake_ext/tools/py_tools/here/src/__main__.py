from .platforms import system
import aexlib.WindowSetup

def window_setup():
    return aexlib.WindowSetup.WindowSetup()

def main():
    w = window_setup()
    w.loop.run()

if __name__ == "__main__":
    main()
