package lab13.task4;

class ConsoleLogger extends LoggerBase
{
    public ConsoleLogger()
    {
        super(LogLevel.all());
    }

    protected void writeMessage(String message)
    {
        System.out.print("[Console]");
        System.out.println(message);
    }
}
