package lab13.task4;

import java.util.EnumSet;

enum LogLevel
{
    Info, Debug, Warning, Error, FunctionalMessage, FunctionalError;

    public static EnumSet<LogLevel> all()
    {
        return EnumSet.allOf(LogLevel.class);
    }
}
