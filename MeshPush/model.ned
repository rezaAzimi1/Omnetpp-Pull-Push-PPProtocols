simple Device
{
    gates:
        input in[];
        output out[];
}


network p2pMesh
{
    @display("bgb=683,394");
    submodules:
        device01: Device {
            @display("p=175,65");
        }
        device02: Device {
            @display("p=86,166");
        }
        device03: Device {
            @display("p=227,166");
        }

        device04: Device {
            @display("p=298,57;i=block/process");
        }
        device05: Device {
            @display("p=561,136");
        }
        device06: Device {
            @display("p=522,283");
        }
        device07: Device {
            @display("p=373,205");
        }
        device08: Device {
            @display("p=312,322");
        }
        device09: Device {
            @display("p=175,283");
        }
        device10: Device {
            @display("p=452,81");
        }
    connections:


        device02.out++ --> {  delay = 10ms; } --> device01.in++;
        device03.out++ --> {  delay = 10ms; } --> device02.in++;
        device01.out++ --> {  delay = 10ms; } --> device02.in++;
        device03.out++ --> {  delay = 10ms; } --> device09.in++;
        device09.out++ --> {  delay = 10ms; } --> device03.in++;
        device03.out++ --> {  delay = 10ms; } --> device04.in++;
        device02.out++ --> {  delay = 10ms; } --> device09.in++;
        device03.out++ --> {  delay = 10ms; } --> device08.in++;
        device08.out++ --> {  delay = 10ms; } --> device03.in++;
        device08.out++ --> {  delay = 10ms; } --> device06.in++;
        device06.out++ --> {  delay = 10ms; } --> device08.in++;
        device06.out++ --> {  delay = 10ms; } --> device07.in++;
        device07.out++ --> {  delay = 10ms; } --> device06.in++;
        device05.out++ --> {  delay = 10ms; } --> device06.in++;
        device05.out++ --> {  delay = 10ms; } --> device07.in++;
        device07.out++ --> {  delay = 10ms; } --> device10.in++;
        device10.out++ --> {  delay = 10ms; } --> device05.in++;
        device07.out++ --> {  delay = 10ms; } --> device04.in++;
        device03.out++ --> {  delay = 10ms; } --> device07.in++;
        device07.out++ --> {  delay = 10ms; } --> device08.in++;
        device10.out++ --> {  delay = 10ms; } --> device04.in++;
        device04.out++ --> {  delay = 10ms; } --> device01.in++;
        device10.out++ --> {  delay = 10ms; } --> device07.in++;
        device01.out++ --> {  delay = 10ms; } --> device04.in++;
        device02.out++ --> {  delay = 10ms; } --> device03.in++;
        device04.out++ --> {  delay = 10ms; } --> device03.in++;
        device09.out++ --> {  delay = 10ms; } --> device02.in++;
        device08.out++ --> {  delay = 10ms; } --> device07.in++;
        device07.out++ --> {  delay = 10ms; } --> device03.in++;
        device04.out++ --> {  delay = 10ms; } --> device07.in++;
        device04.out++ --> {  delay = 10ms; } --> device10.in++;
        device05.out++ --> {  delay = 10ms; } --> device10.in++;
        device06.out++ --> {  delay = 10ms; } --> device05.in++;
        device07.out++ --> {  delay = 10ms; } --> device05.in++;
}
