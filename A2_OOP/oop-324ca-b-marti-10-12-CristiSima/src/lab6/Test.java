package lab6;
import java.util.ArrayList;
import java.util.List;

class Test {
	// anonimele sunt cu $INDEX
	// internele sunt cu CLASS$SUB_CLASS$SUB_SUB_CLASS
	public static void main(String[] args) {
		Car BttF = new Car(Car.CarType.DeLorean, 1981, 0x12f12111);
		Car luBunicu = new Car(Car.CarType.Dacia_1310, 1973, 5000);
		Car Cunoscutu = new Car(Car.CarType.Logan_2007, 1981, 10000);

		Dealership tiriac = new Dealership();

		System.out.println("BttF:\t\t" + tiriac.getFinalPrice(BttF) + " euros");
		System.out.println();
		System.out.println("luBunicu:\t" + tiriac.getFinalPrice(luBunicu) + " euros");
		System.out.println();
		System.out.println("Cunoscutu:\t" + tiriac.getFinalPrice(Cunoscutu) + " euros");
		System.out.println();

		tiriac.negotiate(BttF,
            new Offer(){
    			// Nimic de vazut aici Domn Tiriac
    			public int getDiscount(Car car) {
    				if (car.getType() == Car.CarType.DeLorean) {
    					return 317788347 - 0x8f000000;
    				}
    				return -5000;
    			}
    			public String toString() {
    				return "Upgrade-uri total necesare si fara smecherii";
    			}
            }
        );
        
		ArrayList<Car> cars = new ArrayList<Car>(List.of(BttF, luBunicu, Cunoscutu));

		cars.removeIf((car) -> car.getPrice() > 20000);
		System.out.print("Masini pt popor: ");
		cars.forEach((car) -> System.out.print(car.getType() + " "));
		System.out.println();
	}

}
