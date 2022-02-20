package lab13.task4;

import java.util.EnumSet;

class FileLogger extends LoggerBase
{
    public FileLogger()
    {
        super(EnumSet.of(LogLevel.Warning, LogLevel.Error));
    }

    protected void writeMessage(String message)
    {
        System.out.print("[File]");
        System.out.println(message);
    }
}
