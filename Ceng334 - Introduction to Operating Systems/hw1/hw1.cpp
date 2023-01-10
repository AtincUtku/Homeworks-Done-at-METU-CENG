#include <bits/stdc++.h> 
#include <string.h>
#include "parser.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

class Bundle
{
    public:
        char *name;
        vector<char **> commands;
        Bundle(/* args */);
        ~Bundle();
};

Bundle::Bundle()
{
}

Bundle::~Bundle()
{
    name = NULL;
    commands.clear();
}


int main() 
{
    int pbs_state = 0;
    FILE *file = NULL;
    int quit = 0;
    char *line;
    vector<Bundle> bundles;
    int control = 0;
    int control_for_name = 0;

    while (1)
    {
        line = new char[256];

        fgets(line, 256, stdin);
        parsed_input *parsedInput = new parsed_input();



        parse(line, 0, parsedInput);

        // pbc for create bundles

        if (parsedInput->command.type == PROCESS_BUNDLE_CREATE)
        {
            control = 0;   
            control_for_name = 0;
            
          

            for (int i=0; i<bundles.size(); i++)
            {
                if (!strcmp(bundles[i].name, parsedInput->command.bundle_name))
                {
                    control_for_name = 1;
                    delete parsedInput;
                    delete[] line;
                    break;
                }

            }

            if (control_for_name)
            {
                control_for_name = 0;
                continue;
            }

            Bundle theBundle;
            theBundle.name = parsedInput->command.bundle_name;
            
            delete parsedInput;
            delete[] line;

            while (1)
            {
                line = new char[256];
                
                char **temp_array;

               
                fgets(line, 256, stdin);


               
                parsed_input *parsedInput = new parsed_input();
               
                parse(line, 1, parsedInput);


                // if pbs stop pbc

                if(parsedInput->command.type == PROCESS_BUNDLE_STOP)
                {


                    break;

                }

                // push the commands from argv to commands list inside bundle class

                // for (char **it=parsedInput->argv; *it!=NULL; ++it)
                // {
                //     // again checking for pbs

                //     if(parsedInput->command.type == PROCESS_BUNDLE_STOP)
                //     {
                //         cout<<"------------------"<<endl;

                //         cout<<"Creation stopped m8!!!"<<endl;
                //         cout<<"------------------"<<endl;

                //         control = 1;

                //         break;

                //     }

                //     // cout<<*it<<endl;

                //     //theBundle.commands.push_back(*it);
                // }

                

                theBundle.commands.push_back((parsedInput->argv));


                delete parsedInput;
                delete[] line;

                // if (control) 
                // {
                //     control = 0;
                //     break;
                // }


            }

            // push the created bundle to the bundle list
            

            
            bundles.push_back(theBundle);

             
           
            //for debugging
            // for (int i=0; i < bundles.size(); i++)
            // {
            //     cout<<"------------------"<<endl;

            //     cout<< "Bundle name is : " << bundles[i].name<<endl;

            //     cout<<"------------------"<<endl;

            //     cout<<"Printing commands"<<endl;

            //     for (int j=0; j<bundles[i].commands.size(); j++)
            //     {
            //         cout<<"Argument "<<j<<" is: "<<bundles[i].commands[j]<<endl;
            //     }

            // }
        }


        


        // PBE

        else if (parsedInput->command.type == PROCESS_BUNDLE_EXECUTION)
        {
            
            




            if (parsedInput->command.bundle_count == 1) // only one bundle, no pipelining
            {
                int i;
                int in;
                int out;
                int bundle_exists = 0;



                for (i=0; i<bundles.size(); i++)
                {
                    

                    // cout<<"------------------"<<endl;

                    // cout<<"loopin"<<endl;

                    // cout<<"------------------"<<endl;

                    if (strcmp(bundles[i].name, parsedInput->command.bundles->name) == 0)      
                    {
                        
                        
                        bundle_exists = 1;

                        break;
                    }

                    
                }

                if (!bundle_exists)
                {
                    delete parsedInput;
                    delete[] line;
                    continue;
                }

                // if (parsedInput->command.bundles->input != NULL || parsedInput->command.bundles->input != NULL)
                // {
                //     if (parsedInput->command.bundles->input != NULL)
                //     {
                //         in = open(parsedInput->command.bundles->input, O_RDONLY);

                        
                //     }

                //     if (parsedInput->command.bundles->output != NULL)
                //     {
                //         out = open(parsedInput->command.bundles->output, O_WRONLY | O_RDONLY | O_APPEND);
                //     }
                // }

                //copy(bundles[i].commands.begin(), bundles[i].commands.end(), command_array);


                if (parsedInput->command.bundles->input != NULL)
                    {
                        in = open(parsedInput->command.bundles->input, O_RDONLY);
                        // dup2(in, 0);
                        // close(in);
                            
                    }


                if (parsedInput->command.bundles->output != NULL)
                    {
                        out = open(parsedInput->command.bundles->output, O_APPEND | O_WRONLY | O_CREAT, 0666);
                        // dup2(out, 1);
                        // close(out);
                    }

                for (int j=0; j<bundles[i].commands.size(); j++)
                {


                    if (fork() == 0) // child process
                    {
                        if (parsedInput->command.bundles->input != NULL)
                        {
                            in = open(parsedInput->command.bundles->input, O_RDONLY);
                            // dup2(in, 0);
                            // close(in);
                                
                        }


                        if (parsedInput->command.bundles->output != NULL)
                        {
                            out = open(parsedInput->command.bundles->output, O_APPEND | O_WRONLY | O_CREAT, 0666);
                            // dup2(out, 1);
                            // close(out);
                        }
                        
                        if (parsedInput->command.bundles->input != NULL)
                        {
                            // in = open(parsedInput->command.bundles->input, O_RDONLY);

                            dup2(in, 0);
                            close(in);
                        }

                        if (parsedInput->command.bundles->output != NULL)
                        {
                            // out = open(parsedInput->command.bundles->output, O_WRONLY | O_APPEND);

                            dup2(out, 1);
                            close(out);
                        }


                       
                        
                        execvp(*(bundles[i].commands[j]), (bundles[i].commands[j]));

                    }
                }

                bundles.erase(bundles.begin() + i);

                for (int j=0; j<bundles[i].commands.size(); j++)
                {
                    wait(NULL);
                }





                delete parsedInput;
                delete[] line;
            }

            

            else if (parsedInput->command.bundle_count > 1)  // pipe implementation
            {
                int i;
                int oldIn = dup(0);
                int oldOut = dup(1);
                int in, out;
                int j;
                int bundle_exists=0;
                int c=0;
                int x=0;


                char *buffer;

                

                
                

                if (parsedInput->command.bundles->input != NULL)
                {
                    in = open(parsedInput->command.bundles->input, O_RDONLY);

                    
                }

                else
                {
                    in = dup(oldIn);
                }

                for (i=0; i<parsedInput->command.bundle_count; i++)
                {
                    if (i == parsedInput->command.bundle_count-1)
                    {
                        dup2(in, 0);
                        close(in);
                        
                        if (parsedInput->command.bundles->output != NULL)
                        {
                            out = open(parsedInput->command.bundles->output, O_APPEND | O_WRONLY | O_CREAT, 0666);

                            
                        }

                        else
                        {
                            out = dup(oldOut);
                        }

                        // c=1;

                        dup2(out, 1);
                        close(out);


                    }

                    else
                    {
                        dup2(in, 0);
                        close(in);

                        int fdPipe[2];
                        pipe(fdPipe);

                        in = fdPipe[0];
                        out = fdPipe[1];

                        dup2(out, 1);
                        close(out);
                    }

                    


                    for (j=0; j<bundles.size(); j++)
                    {
                        if (strcmp(bundles[j].name, parsedInput->command.bundles->name) == 0)
                        {
                            bundle_exists = 1;
                            break;
                        }
                    }

                    if (bundle_exists == 0)
                    {
                        continue;
                    }

                    for (int k=0; k<bundles[j].commands.size(); k++)
                    {

                        if (fork()==0)
                        {
                            execvp(*(bundles[j].commands[k]), (bundles[j].commands[k]));
                            
                        }

                    }

                    bundles.erase(bundles.begin() + j);
                    parsedInput->command.bundles++;
                    


                    for (int k=0; k<bundles[j].commands.size(); k++)
                    {
                        wait(NULL);
                    }
                    


                }

                
                dup2(oldOut, 1);
                close(oldOut);    


                for (int k=0; k<bundles[j].commands.size(); k++)
                {
                        wait(NULL); // extra check for clarity
                }

                
                dup2(oldIn, 0);
                close(oldIn);

                delete parsedInput;
                delete[] line;




            }
        }


        

        // quit 
        else if (parsedInput->command.type == QUIT)
        {



            delete parsedInput;
            delete[] line;


            break;
        }
        
        




     

        


    }

    return 0;

}