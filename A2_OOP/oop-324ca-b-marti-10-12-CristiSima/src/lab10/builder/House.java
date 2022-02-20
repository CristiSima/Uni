package lab10.builder;

import java.util.ArrayList;

public class House {
	private int doors;
	private int flors;
	private int windows; // optional(0)  I've never seen one but it's posible
	private int heaterSystemTargetTemperature; // optional(-300)

	private House(HouseBuilder builder) {
		this.doors = builder.doors;
		this.flors = builder.flors;
		this.windows = builder.windows;
		this.heaterSystemTargetTemperature = builder.heaterSystemTargetTemperature;
	}

	public int getDoors() {
		return this.doors;
	}

	public int getFlors() {
		return this.flors;
	}

	public int getWindows() {
		return this.windows;
	}

	public int getHeaterSystemTargetTemperature() {
		return this.heaterSystemTargetTemperature;
	}

	@Override
	public String toString() {
		return "House{ " +
		       "doors: " + this.doors + ", " +
		       "flors: " + this.flors + ", " +
		       "windows: " + this.windows + ", " +
		       "heaterSystemTargetTemperature: " + this.heaterSystemTargetTemperature + " }";
	}


	public static class HouseBuilder {
		private int doors;
		private int flors;
		private int windows = 0; // optional(0)  I've never seen one but it's posible
		private int heaterSystemTargetTemperature = -300; // optional(-300)

		public HouseBuilder(int doors, int flors) {
			this.doors = doors;
			this.flors = flors;
		}

		public HouseBuilder setWindows(int windows) {
			this.windows = windows;
			return this;
		}

		public HouseBuilder setHeaterSystemTargetTemperature(int heaterSystemTargetTemperature) {
			this.heaterSystemTargetTemperature = heaterSystemTargetTemperature;
			return this;
		}

		public House build() {
			return new House(this);
		}
	}
}
