package lab12.reports;

import java.util.*;
import java.util.stream.Collector;
import java.util.stream.Collectors;
import lab12.entities.*;

public class BusinessReport {
    public static Map<Disability, List<Employee>> getEmployeesOnSameDisability(Business business) {
        Map<Disability, List<Employee>> result = new HashMap();
        business.getEmployees().stream().forEach( (employee) -> {
            if (!result.containsKey(employee.getDisability()))
                result.put(employee.getDisability(), new ArrayList());
            result.get(employee.getDisability()).add(employee);
        });
        return result;
    }

    public static long getNumberOfDifferentProjectsWorkedByCurrentFemaleEmployees(Business business) {
        Set<Project> temp = new HashSet();
        business.getEmployees().stream().filter( (e) -> e.getGender() == Gender.FEMALE).forEach( (e) -> e.getProjects().stream().forEach(temp::add));
        return temp.size();
    }

    public static Map<Religion, Map<Gender, List<Employee>>> getEmployeesOnSameReligionAndGender(Business business) {
        return business.getEmployees().stream().collect(Collectors.groupingBy(Employee::getReligion, Collectors.groupingBy(Employee::getGender)));
    }
}
