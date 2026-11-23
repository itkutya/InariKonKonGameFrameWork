export module GenericGPUObject;

import std;

export namespace ikk
{
    class GenericGPUObject
    {
    public:

    private:

    };
}

//TODO:
// - implement a generic rendering object that will store a pointer like object to the GPU representation of the uploaded object
// - CPU objects will "give up" their resources and will only store a pointer like object to this class
// - Handshake will make sure to hand over the resources if the upload was successful,
//   after handshake CPU object stores pointer to this obj, and this obj will store the GPU representation of the given resource