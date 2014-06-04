TreeVis = function() {
  var vis = this;
  this.margin = 75;
  this.width = 600;
  this.height = 500;

  this.i = 0;
  this.duration = 750;

  this.tree = d3.layout.tree()
      .size([this.height, this.width]);

  this.diagonal = d3.svg.diagonal()
      .projection(function(d) { return [d.x, d.y]; });

  this.svg = d3.select(".tree-vis").append("svg")
      .attr("width", vis.width + vis.margin + vis.margin)
      .attr("height", vis.height + vis.margin + vis.margin)
      .append("g")
      .attr("transform", "translate(" + vis.margin + "," + vis.margin + ")");

            var flare = {"name": "Top Level",
                      "parent": "null",
                      "children": [
                        {"name": "Level 2: A",
                         "parent": "Top Level",
                         "children": [
                           {"name": "Son of A",
                            "parent": "Level 2: A" },
                           {"name": "Daughter of A",
                            "parent": "Level 2: A"}
                         ]
                        },
                        {"name": "Level 2: B",
                         "parent": "Top Level"}
                      ]
                     };
  this.root = flare;
  this.root.x0 = this.width / 2;
  this.root.y0 = 0;

  function collapse(d) {
    if (d.children) {
      d._children = d.children;
      d._children.forEach(collapse);
      d.children = null;
    }
  }

  this.root.children.forEach(collapse);

  d3.select(self.frameElement).style("height", "500px");

}

TreeVis.prototype.redraw = function(newTree) {
  this.root = newTree;
  this.root.x0 = this.width / 2;
  this.root.y0 = 0;
  this.update(newTree);
}

TreeVis.prototype.update = function(source) {
  var vis = this;
  // Compute the new tree layout.
  var nodes = vis.tree.nodes(vis.root).reverse(),
      links = vis.tree.links(nodes);

  // Normalize for fixed-depth.
  nodes.forEach(function(d) { d.y = d.depth * 40; });

  // Update the nodes…
  var node = vis.svg.selectAll("g.node")
      .data(nodes, function(d) { return d.id || (d.id = ++vis.i); });

  // Enter any new nodes at the parent's previous position.
  var nodeEnter = node.enter().append("g")
      .attr("class", "node")
      .attr("transform", function(d) { return "translate(" + source.x0 + "," + source.y0 + ")"; })

  nodeEnter.append("circle")
      .attr("r", 1e-6)
      .style("fill", function(d) { return d.name == "NULLNULLNULL" ? "#000" : "#fff";
      });

  nodeEnter.append("text")
      .attr("x", function(d) { return d.children || d._children ? -10 : 10; })
      .attr("dy", ".35em")
      .attr("text-anchor", function(d) { return d.children || d._children ? "end" : "start"; })
      .text(function(d) { return d.name; })
      .style("fill-opacity", 1e-6);

  // Transition nodes to their new position.
  var nodeUpdate = node.transition()
      .duration(vis.duration)
      .attr("transform", function(d) { return "translate(" + d.x + "," + d.y + ")"; });

  nodeUpdate.select("circle")
      .attr("r", 4.5)
      .style("fill", function(d) { return d.name == "NULLNULLNULL" ? "#fff" : "#fff"; });

  nodeUpdate.select("text")
      .style("fill-opacity", 1);

  // Transition exiting nodes to the parent's new position.
  var nodeExit = node.exit().transition()
      .duration(vis.duration)
      .attr("transform", function(d) { return "translate(" + source.x + "," + source.y + ")"; })
      .remove();

  nodeExit.select("circle")
      .attr("r", 1e-6);

  nodeExit.select("text")
      .style("fill-opacity", 1e-6);

  // Update the links…
  var link = vis.svg.selectAll("path.link")
      .data(links, function(d) { return d.target.id; });

  // Enter any new links at the parent's previous position.
  link.enter().insert("path", "g")
      .attr("class", "link")
      .attr("d", function(d) {
        var o = {x: source.x0, y: source.y0};
        return vis.diagonal({source: o, target: o});
      });

  // Transition links to their new position.
  link.transition()
      .duration(vis.duration)
      .attr("d", vis.diagonal);

  // Transition exiting nodes to the parent's new position.
  link.exit().transition()
      .duration(vis.duration)
      .attr("d", function(d) {
        var o = {x: source.x, y: source.y};
        return vis.diagonal({source: o, target: o});
      })
      .remove();

  // Stash the old positions for transition.
  nodes.forEach(function(d) {
    d.x0 = d.x;
    d.y0 = d.y;
  });
}
