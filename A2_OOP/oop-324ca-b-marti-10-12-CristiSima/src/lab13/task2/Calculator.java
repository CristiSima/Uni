package lab13.task2;

import java.util.Collection;

public class Calculator {
    public static class NullParameterException extends RuntimeException {
    }

    public static class UnderflowException extends RuntimeException {
    }

    public static class OverflowException extends RuntimeException {
    }

    void check(Double nr)
    {
        if(nr==null)
            throw new Calculator.NullParameterException();
        if(nr==Double.POSITIVE_INFINITY)
            throw new Calculator.OverflowException();
        if(nr==Double.NEGATIVE_INFINITY)
            throw new Calculator.UnderflowException();
    }

    Double add(Double nr1, Double nr2)
    {
        check(nr1);check(nr2);check(nr1+nr2);
        return nr1+nr2;
    }

    Double divide(Double nr1, Double nr2)
    {
        check(nr1);check(nr2);check(nr1/nr2);
        return nr1/nr2;
    }

    Double average(Collection<Double> numbers)
    {

        return this.divide(numbers.stream().reduce(0d, this::add), Double.valueOf(numbers.size()));
    }
}
