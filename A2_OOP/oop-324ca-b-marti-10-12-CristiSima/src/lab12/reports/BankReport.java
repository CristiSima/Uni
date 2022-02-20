package lab12.reports;

import java.util.*;
import java.util.Map.Entry;
import java.util.function.Function;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;
import lab12.entities.*;

public class BankReport {
    // Customer = Employee of the Business
    // Business = a client of the Bank
    // Customers of the Bank = all the Employees that work for the Businesses that are clients of the Bank

    public static int getNumberOfCustomers(Bank bank) {
        return bank.getClients().stream.map( (business) -> business.getEmployees().size()).reduce(0, Integer::sum);
    }

    public static int getNumberOfAccounts(Bank bank) {
        return bank.getClients().size();
    }

    public static SortedSet<Employee> getCustomersSorted(Bank bank) {
        SortedSet<Employee> result = new TreeSet(Comparator.comparing(Employee::getName));
        bank.getClients().stream().forEach( (business) -> business.getEmployees().forEach(result::add) );
        return result;
    }

    public static double getTotalSumInAccounts(Bank bank) {
        return bank.getClients().stream().map( (business) -> business.getEmployees().stream().map( (employee) -> employee.getAccounts().stream().map(Account::getBalance).reduce(0d, Double::sum) ).reduce(0d, Double::sum)
                ).reduce(0d, Double::sum);
    }

    public static SortedSet<Account> getAccountsSortedBySum(Bank bank) {
        SortedSet<Account> result = new TreeSet(Comparator.comparing(Account::getBalance));
        bank.getClients().stream().forEach( (business) -> business.getEmployees().stream().forEach( (employee) -> result.addAll(employee.getAccounts()) ));
        return result;
    }

    public static Map<Employee, Collection<Account>> getCustomerAccounts(Bank bank) {
        Map<Employee, Collection<Account>> result = new HashMap();
        bank.getClients().stream().forEach( (business) -> result.putAll(business.getEmployees().stream().collect(Collectors.toMap( (employee) -> employee, (employee) -> employee.getAccounts() ) )) );
        return result;
    }

    public static Map<String, List<Employee>> getCustomersByCity(Bank bank) {
        Map<String, List<Employee>> result = new HashMap();
        bank.getClients().stream().forEach( (business) -> business.getEmployees().stream().forEach( (employee) -> {
            if (!result.containsKey(employee.getCity()))
                result.put(employee.getCity(), new ArrayList());
            result.get(employee.getCity()).add(employee);
        }));
        return result;
    }
}
