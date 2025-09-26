Node* keysWithPrefix( Nodo* root, char* pre ){
    Nodo* puppet = root;

    while ( *word ) {
        int l = *word - 'a';

        if ( !puppet->next[l] ) {
            return NULL;
        }
        puppet = puppet->next[l];
        word++;
    }
   
    return puppet;
}