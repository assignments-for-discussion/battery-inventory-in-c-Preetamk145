#include <stdio.h>
#include <assert.h>

struct CountsBySoH {
  int healthy;
  int exchange;
  int failed;
};

struct CountsBySoH countBatteriesByHealth(const int* presentCapacities, int nBatteries) {
  struct CountsBySoH counts = {0, 0, 0};
  for (int i = 0; i < nBatteries; ++i) {
    // Convert present capacity to SoH
    double SoH = 100.0 * presentCapacities[i] / 120.0;

    // Classify using the specified ranges
    if (SoH > 80.0) {
      counts.healthy++;
    } else if (SoH <= 80.0 && SoH >= 62.0) {
      counts.exchange++;
    } else {
      counts.failed++;
    }
  }
  return counts;
}

void testBucketingByHealth() {
  const int presentCapacities[] = {113, 116, 80, 95, 92, 70};
  const int numberOfBatteries = sizeof(presentCapacities) / sizeof(presentCapacities[0]);
  printf("Counting batteries by SoH...\n");
  struct CountsBySoH counts = countBatteriesByHealth(presentCapacities, numberOfBatteries);
  assert(counts.healthy == 2);
  assert(counts.exchange == 3);
  assert(counts.failed == 1);

  // Additional asserts
  assert(counts.healthy + counts.exchange + counts.failed == numberOfBatteries);  // Total counts should equal the number of batteries
  assert(counts.healthy >= 0 && counts.exchange >= 0 && counts.failed >= 0);  // Counts should not be negative
  assert(counts.healthy <= numberOfBatteries && counts.exchange <= numberOfBatteries && counts.failed <= numberOfBatteries);  // Counts should not exceed the number of batteries

  // New tests cases
  const int maxCapacity = 120;
  const int minCapacity = 0;

  // Test with the maximum rated capacity 
  const int maxCapacityTest[] = {maxCapacity, maxCapacity, maxCapacity};
  struct CountsBySoH maxCapacityCounts = countBatteriesByHealth(maxCapacityTest, 3);
  assert(maxCapacityCounts.healthy == 3);
  assert(maxCapacityCounts.exchange == 0);
  assert(maxCapacityCounts.failed == 0);

  // Test with the minimum rated capacity 
  const int minCapacityTest[] = {minCapacity, minCapacity, minCapacity};
  struct CountsBySoH minCapacityCounts = countBatteriesByHealth(minCapacityTest, 3);
  assert(minCapacityCounts.healthy == 0);
  assert(minCapacityCounts.exchange == 0);
  assert(minCapacityCounts.failed == 3);

  // Test with values at the boundary between healthy and exchange
  const int boundaryTest[] = {80, 80, 62, 62};
  struct CountsBySoH boundaryCounts = countBatteriesByHealth(boundaryTest, 4);
  assert(boundaryCounts.healthy == 2);
  assert(boundaryCounts.exchange == 2);
  assert(boundaryCounts.failed == 0);

  // Test with negative values (boundary test)
  const int negativeTest[] = {-10, 30, -60};
  struct CountsBySoH negativeCounts = countBatteriesByHealth(negativeTest, 3);
  assert(negativeCounts.healthy == 1);
  assert(negativeCounts.exchange == 1);
  assert(negativeCounts.failed == 1);

  printf("All tests passed. Done counting :)\n");
}

int main() {
  testBucketingByHealth();
  return 0;
}
