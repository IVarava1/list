#include <stdio.h>

#include "../inc/list_struct.h"
#include "../errors/errors.h"
#include "../inc/list_dump.h"

int dump_counter = 0;

void List_dump_( list_t* list, Var_info varinfo ) 
{
    FILE* dump_file = fopen("dump.html", "a");

    fprintf(dump_file, "<h2 style=\"color: green\"> ListDump called from %s, function: %s, line %d, list name: %s</h2>\n", varinfo.file_name, varinfo.func_name, varinfo.line, varinfo.name);

    if (list == NULL) {
        fprintf(dump_file, "<h3 style=\"color: red\">List pointer equal NULL</h3>\n");
        return;
    }

    Text_listing(list, dump_file);

    Image_list_dump(list, "graph.dot");

    char img_path[100] = {};
    sprintf(img_path, "result%d.png", dump_counter++);

    fprintf(dump_file, "<img src=\"%s\">\n", img_path);

    fprintf(dump_file, "<hr>\n");

    fclose(dump_file);
}

void Text_listing( list_t* list, FILE* dump_file ) 
{
    fprintf(dump_file, "<pre>-----------------------------------------------\n");
    fprintf(dump_file, "|  index  |    value  |     prev  |     next  |\n");
    fprintf(dump_file, "-----------------------------------------------\n");
    
    for (int i = 0; i <= list -> capacity + 1; ++i) {
        fprintf(dump_file, "|%7d  |  %7d  |  %7d  |  %7d  |\n", i, list -> data[i].value, list -> data[i].prev, list -> data[i].next);
        fprintf(dump_file, "-----------------------------------------------\n");
    }
    
    fprintf(dump_file, "</pre>\n");
}

void Image_list_dump( list_t* list, const char* file_name ) 
{
    FILE* graph = fopen(file_name, "w");

    fprintf(graph, "digraph G{\n");
    fprintf(graph, "\trankdir = LR;\n");
    fprintf(graph, "\tnode[shape = Mrecord, fontsize = 17, style = \"filled\", fillcolor = \"#ffff00ff\"];\n");
    fprintf(graph, "\tbgcolor = \"#343434ff\";\n");
    fprintf(graph, "\tsplines = ortho;\n");

    if (list->data && list -> data[0].value != CANARY_LEFT)
        fprintf(graph, "\t%d[shape = Mrecord, fillcolor = \"#9f2147ff\", label = \"{<f0> idx: %d} | {<f1> data: %X} | {{<f2> next: %d} | {<f3> prev: %d}}\"];\n", 0, 0, list->data[0].value, list->data[0].next, list->data[0].prev);
    else
        fprintf(graph, "\t%d[shape = Mrecord, fillcolor = \"#7edef8ff\", label = \"{<f0> idx: %d} | {<f1> data: %X} | {{<f2> next: %d} | {<f3> prev: %d}}\"];\n", 0, 0, list->data[0].value, list->data[0].next, list->data[0].prev);
    for (size_t i = 1; i <= list->capacity; ++i) {
        if (list -> data[i].prev != -1)
            fprintf(graph, "\t%d[shape = Mrecord, fillcolor = \"#ffff00ff\", label = \"{<f0> idx: %d} | {<f1> data: %d} | {{<f2> next: %d} | {<f3> prev: %d}}\"];\n", i, i, list->data[i].value, list->data[i].next, list->data[i].prev);
        else
            fprintf(graph, "\t%d[shape = Mrecord, fillcolor = \"#00ff7bff\", label = \"{<f0> idx: %d} | {<f1> data: %X} | {{<f2> next: %d} | {<f3> prev: %d}}\"];\n", i, i, list->data[i].value, list->data[i].next, list->data[i].prev);
    }

    fprintf(graph, "\tfree[shape = Mrecord, fillcolor = \"#eb3eb4ff\"];\n");
    fprintf(graph, "\ttail[shape = Mrecord, fillcolor = \"#ade545ff\"];\n");
    fprintf(graph, "\thead[shape = Mrecord, fillcolor = \"#ff9500ff\"];\n");
    fprintf(graph, "\thead->tail->free[style = invis];\n");

    for (int i = 1; i <= list -> capacity; ++i) {
        fprintf(graph, "%d->%d[style = invis];\n", i - 1, i);
    }

    int i = 0;
    int head_next = -1;
    int last = i;
    for (i = list -> data[i].next; i != 0; i = list -> data[i].next) {
        if (i == -1 || i > list -> capacity)
            break;

        fprintf(graph, "\t%d->%d[style = invis, constraint = false];\n", last, i);
        
        if (last == 0)
            head_next = i;

        last = i;
    }

    fprintf(graph, "\tedge[style = \"\", constraint = false];\n");
    fprintf(graph, "\tnode[shape = octagon, style = \"filled\", fillcolor=\"#ff3030ff\"];\n");

    i = list->free;
    last = i;

    if (i != POIZON_IND) {
        fprintf(graph, "\tfree->%d[color = \"#eb3eb4ff\"]\n", i);
        for (i = list -> data[i].next; i != POIZON_IND; i = list -> data[i].next) {
            if (i == -1 || i > list->capacity)
                break;

            fprintf(graph, "\t%d->%d[color = \"#eb3eb4ff\"];\n", last, i);
            last = i;
        }

        int rcan_ind = list->capacity + 1;
        if (list -> data && list -> capacity >= -1 && list -> data[list -> capacity + 1].value != CANARY_RIGHT)
           fprintf(graph, "\tRCAN[shape = Mrecord, fillcolor = \"#9f2147ff\", label = \"{<f0> idx: %d} | {<f1> data: %X} | {{<f2> next: %d} | {<f3> prev: %d}}\"];\n", rcan_ind, list -> data[rcan_ind].value, list -> data[rcan_ind].next, list -> data[rcan_ind].prev);
        else 
            fprintf(graph, "\tRCAN[shape = Mrecord, fillcolor = \"#7edef8ff\", label = \"{<f0> idx: %d} | {<f1> data: %X} | {{<f2> next: %d} | {<f3> prev: %d}}\"];\n", rcan_ind, list -> data[rcan_ind].value, list -> data[rcan_ind].next, list -> data[rcan_ind].prev);
        fprintf(graph, "\t%d->RCAN[style = \"invis\"]\n", last);
    }
    else {
        int rcan_ind = list->capacity + 1;
        if (list -> data && list->capacity >= -1 && list -> data[list -> capacity + 1].value != CANARY_RIGHT)
           fprintf(graph, "\tRCAN[shape = Mrecord, fillcolor = \"#9f2147ff\", label = \"{<f0> idx: %d} | {<f1> data: %X} | {{<f2> next: %d} | {<f3> prev: %d}}\"];\n", rcan_ind, list -> data[rcan_ind].value, list -> data[rcan_ind].next, list -> data[rcan_ind].prev);
        else 
            fprintf(graph, "\tRCAN[shape = Mrecord, fillcolor = \"#7edef8ff\", label = \"{<f0> idx: %d} | {<f1> data: %X} | {{<f2> next: %d} | {<f3> prev: %d}}\"];\n", rcan_ind, list -> data[rcan_ind].value, list -> data[rcan_ind].next, list -> data[rcan_ind].prev);
        fprintf(graph, "\tfree->RCAN[style = \"invis\"]\n", last);
    }

    fprintf(graph, "\thead->%d[color = \"#ff9500ff\"];\n", list -> data[0].next);
    fprintf(graph, "\ttail->%d[color = \"#ade545ff\"];\n", list -> data[0].prev);

    fprintf(graph, "\t%d->RCAN[constraint = true, style = invis];\n", list -> capacity);

    for (i = 0; i <= list -> capacity; ++i) {
        if (list -> data[i].prev == -1)
            continue;

        int next = list -> data[i].next;
        int prev = list -> data[i].prev;

        if (next != POIZON_IND) {
            if (0 <= next && next <= list -> capacity + 1) {
                if (list -> data[next].prev == i) {
                    if (i < next)
                        fprintf(graph, "\t%d->%d[color = \"#ffffffff\", dir = \"both\"];\n", i, next);
                }
                else {
                    fprintf(graph, "\t%d->%d[color = \"#8775d7ff\"]", i, next);
                }
            }
            else {
                fprintf(graph, "\t%d->%d[penwidth = 5, color = \"#8775d7ff\"]", i, next);
            }
        }
        
        if (prev != POIZON_IND) {
            if (0 <= prev && prev <= list -> capacity + 1) {
                if (list -> data[prev].next == i) {
                    if (i < prev)
                        fprintf(graph, "\t%d->%d[color = \"#ffffffff\", dir = \"both\"];\n", i, prev);
                }
                else {
                    fprintf(graph, "\t%d->%d[color = \"#25d876ff\"]", i, prev);
                }
            }
            else {
                fprintf(graph, "\t%d->%d[penwidth = 5, color = \"#25d876ff\"]", i, prev);
            }
        }
    }

    if (MAX_LIST_SIZE >= list -> size && list -> size >= 0)
        fprintf(graph, "\tsize[shape = Mrecord, style = \"filled\", fillcolor = \"#4a9b62ff\", label = \"size = %d\"]\n", list -> size);
    else
        fprintf(graph, "\tsize[shape = Mrecord, style = \"filled\", fillcolor = \"#ad5050ff\", label = \"size = %d\"]\n", list -> size);

    if (MAX_LIST_SIZE >= list -> capacity && list -> capacity >= 0)
        fprintf(graph, "\tcapacity[shape = Mrecord, style = \"filled\", fillcolor = \"#4a9b62ff\", label = \"capacity = %d\"]\n", list -> capacity);
    else
        
        fprintf(graph, "\tcapacity[shape = Mrecord, style = \"filled\", fillcolor = \"#ad5050ff\", label = \"capacity = %d\"]\n", list -> capacity);

    fprintf(graph, "\tedge[style = invis];\n");
    fprintf(graph, "\tcapacity->size[constraint = true];\n");
    
    fprintf(graph, "}\n");
    fclose(graph);

    char dot_str[100] = {};
    sprintf(dot_str, "dot %s -T png -o result%d.png", file_name, dump_counter);
    system(dot_str);
}

void Clear_dump_html()
{
    FILE* file_dump = fopen("dump.html", "w");
    fclose(file_dump);
}