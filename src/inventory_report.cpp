#include "inventory_report.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

bool isValidQuantity(int quantity) {
  return (quantity >= 0);
}

bool isValidPrice(double price) {
  return (price >= 0);
}

double calculateItemValue(const InventoryItem& item) {
  if (isValidPrice(item.price) and isValidQuantity(item.quantity)) {
      return item.price * item.quantity;
  }

  return 0.0;
}

int readInventoryFile(string filename, InventoryItem items[], int maxItems) {
    if (items == nullptr || maxItems <= 0) {
        return 0;
    }

    ifstream in(filename);

    if (!in.is_open()) {
        return 0;
    }

    int count = 0;
    InventoryItem item;

    while (count < maxItems &&
           in >> item.sku >> item.name >> item.quantity >> item.price) {

        if (isValidQuantity(item.quantity) && isValidPrice(item.price)) {
            items[count] = item;
            count++;
        }
    }

    in.close();

    return count;
}

bool writeInventoryReport(string filename, const InventoryItem items[], int count) {
    if (items == nullptr || count < 0) {
        return false;
    }

    ofstream out(filename);

    if (!out.is_open()) {
        return false;
    }

    // SUGGESTION 1:
    // Consider using '\n' instead of endl because endl flushes
    // the output stream every time, which is unnecessary here.
    out << "Inventory Report" << '\n';
    out << "SKU Name Quantity Price Value" << '\n';

    for (int i = 0; i < count; i++) {
        out << items[i].sku << " "
            << items[i].name << " "
            << items[i].quantity << " "
            << items[i].price << " "
            << calculateItemValue(items[i])
            << '\n';
    }

    out << "Total inventory value: "
        << calculateTotalInventoryValue(items, count)
        << '\n';

    out.close();

    return true;
}

double calculateTotalInventoryValue(const InventoryItem items[], int count) {

   if (items == nullptr or count <= 0) return 0.0;

   double sum = 0.0;

   for (int i = 0; i < count; ++i) {
     sum = sum + calculateItemValue(items[i]);
   }

   return sum;
}

int findItemBySku(const InventoryItem items[], int count, string sku) {

   if (items == nullptr or count <= 0) return -1;

   for (int i = 0; i < count; ++i) {
     if (items[i].sku == sku) return i;
   }

   return -1;
}

int findHighestValueItemIndex(const InventoryItem items[], int count) {
   if (items == nullptr or count <= 0) return -1;

   int maxIndex = 0;

   // SUGGESTION 2:
   // Compare total item value instead of only price, since "highest value"
   // should include both quantity and price.
   for (int i = 1; i < count; ++i) {
     if (calculateItemValue(items[i]) >
         calculateItemValue(items[maxIndex])) {
       maxIndex = i;
     }
   }

   return maxIndex;
}