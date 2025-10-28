class Train:
    def __init__(self, number, dest, time):
        self.number = number
        self.dest = dest
        self.time = time


class TrainNode:
    def __init__(self, train):
        self.train = train
        self.left = None
        self.right = None


class Depo:
    def __init__(self):
        self.root = None

    def add(self, train):
        if self.exists(train.number):
            print(f"Train with number {train.number} already exists.")
        else:
            self.root = self._add_recursive(self.root, train)
            print("Train added.")

    def _add_recursive(self, current_node, train):
        if current_node is None:
            return TrainNode(train)
        if train.number < current_node.train.number:
            current_node.left = self._add_recursive(current_node.left, train)
        else:
            current_node.right = self._add_recursive(current_node.right, train)
        return current_node

    def showAll(self):
        self._inorder_traversal(self.root)

    def _inorder_traversal(self, current_node):
        if current_node:
            self._inorder_traversal(current_node.left)
            self.trainInfo(current_node.train)
            self._inorder_traversal(current_node.right)

    def searchByNumber(self, train_number):
        self._search_by_number_recursive(self.root, train_number)

    def _search_by_number_recursive(self, current_node, train_number):
        if current_node is None:
            print(f"Train number {train_number} not found.")
            print()
        elif current_node.train.number == train_number:
            self.trainInfo(current_node.train)
        elif train_number < current_node.train.number:
            self._search_by_number_recursive(current_node.left, train_number)
        else:
            self._search_by_number_recursive(current_node.right, train_number)

    def searchByDest(self, dest):
        self._search_by_dest_recursive(self.root, dest)

    def _search_by_dest_recursive(self, current_node, dest):
        if current_node:
            if current_node.train.dest == dest:
                self.trainInfo(current_node.train)
            self._search_by_dest_recursive(current_node.left, dest)
            self._search_by_dest_recursive(current_node.right, dest)

    def exists(self, train_number):
        return self._exists_recursive(self.root, train_number)

    def _exists_recursive(self, current_node, train_number):
        if current_node is None:
            return False
        if current_node.train.number == train_number:
            return True
        if train_number < current_node.train.number:
            return self._exists_recursive(current_node.left, train_number)
        else:
            return self._exists_recursive(current_node.right, train_number)

    def trainInfo(self, train):
        print(f"Train number: {train.number}")
        print(f"Destination: {train.dest}")
        print(f"Departure time: {train.time}")
        print()


def main():
    depo = Depo()

    while True:
        print("Menu:")
        print()
        print("1. Add train")
        print("2. All trains")
        print("3. Search by number")
        print("4. Search by destination")
        print("0. Exit")
        print()
        choice = input("Enter choice: ")
        print()

        if choice == "1":
            number = input("Enter train number: ")
            dest = input("Enter train destination: ")
            time = input("Enter departure time: ")
            print()
            train = Train(number, dest, time)
            depo.add(train)
            print()
        elif choice == "2":
            depo.showAll()
        elif choice == "3":
            train_number = input("Enter train number: ")
            print()
            depo.searchByNumber(train_number)
        elif choice == "4":
            dest = input("Enter train destination: ")
            print()
            depo.searchByDest(dest)
        elif choice == "0":
            break
        else:
            print("Error. Try again.")
            print()


if __name__ == "__main__":
    main()