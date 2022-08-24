#include "RP_Enumeration.hh"

// Insert here the code in the method of class EnumerationWLOpt
void EnumerationRPOpt::First()
{ // insert first router

    point router;
    router.row = 0;
    router.col = 0;

    while (in.IsWall(router.row, router.col))
    {
        if (router.col < in.Columns()-1)
            router.col++;
        else if (router.row < in.Rows()-1 && router.col == in.Columns()-1) 
            router.row++;
    }

    out.InsertRouter(router.row, router.col);
    out.ConnectRouterWithBackbone(router);
}


bool EnumerationRPOpt::Next()
{  
    unsigned cycle = 0;

    // get coordinates of last router inserted
    unsigned r = out.Router(out.RoutersSize()-1).row;
    unsigned c = out.Router(out.RoutersSize()-1).col;


    if (r == in.Rows()-1 && c == in.Columns()-1) // LastOfCycle()
    {
        unsigned i;

        cout << "In next cycle" << endl;
        cycle++;
        if (out.RoutersSize() == 1)
            return false;

        // remove all routers
        out.RemoveAllRoutersAndRestartBackbone();
        for (unsigned rt = 0; rt < out.RoutersSize(); rt++)
            cout << "router[" << rt << "].row: " << out.Router(rt).row << " " << "router[" << rt << "].col: " << out.Router(rt).col << endl;

        cout << " and now : " << endl;
        cout << out;

        r = 0;
        c = 0;

        // change first in next cycle
        unsigned iteration = 0;
        do
        {
            do
            {
                c < in.Columns()-1 ? c++ : r++;
            } while (in.IsWall(r,c));

            iteration++;
        } while (iteration < cycle);
           
        out.InsertRouter(r,c);
        out.UpdateBackbone();

        cout << "r: " << r << " " << "c: " << c << endl;
        cout << out;
    }
    
    if (c < in.Columns()-1) // NextInSameRow()
    {
        out.RemoveRouter(out.Router(out.RoutersSize()-1));
        out.UpdateBackbone();

        // skip walls
        do
        {
            c++;
        } while (in.IsWall(r,c));

        out.InsertRouter(r,c);
        out.UpdateBackbone();

        cout << "r: " << r << " " << "c: " << c << endl;
    }
    else if (r < in.Rows()-1 && c == in.Columns()-1) // NextInNewRow()
    {
        out.RemoveRouter(out.Router(out.RoutersSize()-1));
        out.UpdateBackbone();

        c = 0;
        r++;

        // skip walls
        while (in.IsWall(r,c))
            c++;
        
        out.InsertRouter(r,c);
        out.UpdateBackbone();

        cout << "r: " << r << " " << "c: " << c << endl;
    }

    return true;
}


bool EnumerationRPOpt::Feasible()
{
    // check budget
    if (out.RemainingBudget() < 0)
        return false;

    // check routers
    for (unsigned i = 0; i < out.RoutersSize(); i++)
        if (in.IsWall(out.Router(i).row, out.Router(i).col))
            return false;
    
    return true;
}


unsigned EnumerationRPOpt::Cost()
{
    if (out.TotalCoveredPoints() == 0)
        return out.RoutersSize()*in.RouterPrice()+out.BackboneSize()*in.BackbonePrice();
    else
        return out.RoutersSize()*in.RouterPrice()+out.BackboneSize()*in.BackbonePrice()-out.TotalCoveredPoints();

}

