1)  #!/bin/bash

address_book_file="address_book.txt"

create_address_book() {
    touch "$address_book_file"
    echo "Address book created successfully."
}

view_address_book() {
    if [ -s "$address_book_file" ]; then
        cat "$address_book_file"
    else
        echo "Address book is empty."
    fi
}

insert_record() {
    echo "Enter the details for the new record:"
    echo -n "Name: "
    read name
    echo -n "Address: "
    read address
    echo -n "Phone: "
    read phone

    echo "Name: $name, Address: $address, Phone: $phone" >> "$address_book_file"
    echo "Record inserted successfully."
}

delete_record() {
    echo "Enter the name of the record to delete:"
    read name

    if grep -q "$name" "$address_book_file"; then
        sed -i "/$name/d" "$address_book_file"
        echo "Record deleted successfully."
    else
        echo "Record not found."
    fi
}

modify_record() {
    echo "Enter the name of the record to modify:"
    read name

    if grep -q "$name" "$address_book_file"; then
        echo "Enter the new details for the record:"
        echo -n "Address: "
        read address
        echo -n "Phone: "
        read phone

        sed -i "s/$name.*/Name: $name, Address: $address, Phone: $phone/" "$address_book_file"
        echo "Record modified successfully."
    else
        echo "Record not found."
    fi
}

while true; do
    echo "Options:"
    echo "a) Create address book"
    echo "b) View address book"
    echo "c) Insert a record"
    echo "d) Delete a record"
    echo "e) Modify a record"
    echo "f) Exit"
    echo -n "Enter your choice: "
    read choice

    case $choice in
        a) create_address_book;;
        b) view_address_book;;
        c) insert_record;;
        d) delete_record;;
        e) modify_record;;
        f) echo "Exiting program. Goodbye!"; break;;
        *) echo "Invalid option. Please try again.";;
    esac
done
