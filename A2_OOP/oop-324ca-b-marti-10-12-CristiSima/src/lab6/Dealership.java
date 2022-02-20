package lab6;

import java.util.List;
import java.util.Random;

class Dealership {
	class BrandOffer implements Offer {
		public int getDiscount(Car car) {
			switch (car.getType()) {
                
			case DeLorean:
				return 0;

			case Dacia_1310:
				return (int) 0.1 * car.getPrice();

			case Logan_2007:
				return (int) 0.05 * car.getPrice();
			default:
				return 0;
			}
		}

		public String toString() {
			return "Brand";
		}
	}
	class DealerOffer implements Offer {
		public int getDiscount(Car car) {
			return (int) (2021 - car.getFabricationYear()) * 100;
		}

		public String toString() {
			return "Dealer";
		}
	}
	class SpecialOffer implements Offer {
		public int getDiscount(Car car) {
			Random random = new Random();
			return random.nextInt(300);
		}

		public String toString() {
			return "Special";
		}
	}

	public int getFinalPrice(Car car) {
		int price = car.getPrice();
		int discount;
		for (Offer offer:List.of(new BrandOffer(), new DealerOffer(), new SpecialOffer())) {
			discount = offer.getDiscount(car);
			System.out.println("Applying " + offer + " discount: " + discount + " euros");
			price -= discount;
		}
		return price;
	}

	void negotiate(Car car, Offer offer) {
		int price = this.getFinalPrice(car);
		System.out.println("Final price: " + price);

		int discount = offer.getDiscount(car);
		System.out.println("Applying Client discount: " + discount + " euros");

		price -= discount;
		System.out.println("Final price after negotiation: " + price + " euros");
	}
}
