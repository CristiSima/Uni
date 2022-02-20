package lab13.task4;

import java.util.EnumSet;

class EmailLogger extends LoggerBase
{
    public EmailLogger()
    {
        super(EnumSet.of(LogLevel.FunctionalMessage, LogLevel.FunctionalError));
    }

    protected void writeMessage(String message)
    {
        System.out.print("[Email]");
        System.out.println(message);
    }
}
