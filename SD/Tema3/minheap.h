#include <stdio.h>

template<typename T> class Heap {
    public:
        T *H;
        int currentDim, maxDim;

        Heap(int maxDim) {
            this->maxDim = maxDim;
            H = new T[this->maxDim + 1];
            currentDim = 0;
        }

        void insertElement(T x) {
            if (currentDim == maxDim) {
                fprintf(stderr, "Error!\n");
                return;
            }
            currentDim++;
            H[currentDim] = x;
            pushUp(currentDim);
}
        T peek() {
            if (currentDim == 0) {
                T x;
		x.frecv=0; //conditia de verificare daca heapul e gol e frecv!=0
                return x;
            }

            return H[1];
        }

        T extractMin() {
            if (currentDim == 0) {
                fprintf(stderr, "Error!\n");
                T x;
                return x;
            }

            T minValue = H[1];
            H[1] = H[currentDim];
            currentDim--;
            if (currentDim > 0)
                pushDown();

            return minValue;
        }

        void pushUp(int l) {
            int parent;
            T vaux;

            parent = l / 2;
            while (l > 1 && H[parent].frecv >= H[l].frecv) { //>= in loc de >
                vaux = H[parent];
                H[parent] = H[l];
                H[l] = vaux;

                l = parent;
                parent = l / 2;
            }
        }
        void pushDown() {
            int l = 1;
            T vaux;

            while (1) {
                if (2 * l + 1 > currentDim) {
                    if (2 * l > currentDim)
                        break;
                    else if (H[2 * l].frecv < H[l].frecv) {
                        vaux = H[2 * l];
                        H[2 * l] = H[l];
                        H[l] = vaux;
                        l = 2 * l;
                    }
                    else
                        break;
                }
                else {
                    if (H[2 * l].frecv <= H[2 * l + 1].frecv && H[2 * l].frecv < H[l].frecv) {
                        vaux = H[2 * l];
                        H[2 * l] = H[l];
                        H[l] = vaux;
                        l = 2 * l;
                    }
                    else
                    if (H[2 * l + 1].frecv <= H[2 * l].frecv && H[2 * l + 1].frecv < H[l].frecv) {
                        vaux = H[2 * l + 1];
                        H[2 * l + 1] = H[l];
                        H[l] = vaux;
                        l = 2 * l + 1;
                    }
                    else
                        break;
                }
            }
        }
};

