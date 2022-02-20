package lab13.task4;

import java.util.EnumSet;

abstract class LoggerBase
{
    private LoggerBase next;
    private EnumSet<LogLevel> loggerLevels;

    public LoggerBase(EnumSet<LogLevel> loggerLevels)
    {
        this.loggerLevels=loggerLevels;
    }

    public void setNext(LoggerBase next)
    {
        this.next=next;
    }

    abstract protected void writeMessage(String message);

    public void message(String message, LogLevel logLevel)
    {
        if(this.loggerLevels.contains(logLevel))
            this.writeMessage(message);
        if(this.next!=null)
            this.next.message(message, logLevel);
    }
}
